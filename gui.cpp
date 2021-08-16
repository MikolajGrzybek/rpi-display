#include "gui.hpp"

static const double PI = 3.14159265359;
static const double DEG2RAD = (180.0 / PI);

extern IT8951_Dev_Info Dev_Info;

extern UDOUBLE Init_Target_Memory_Addr;

Gui::Gui()
    :gps(new Gps(GPS_PORT)),
     m_can(new CAN(CAN_ITF)),
     m_mav("10.0.0.1")
{
    m_start_time = std::chrono::high_resolution_clock::now();

    m_can->start_receiving();
    connect(m_can, &CAN::frame_received, this, &Gui::on_can_frame_received);

    connect(gps, &Gps::position_updated, this, &Gui::on_gps_data_received);
    connect(gps, &Gps::fixChanged, this, &Gui::on_gps_fix_changed);

    InitGui();
}


extern Mavlink * g_mav;
void Gui::on_gps_data_received(sbt_RMC_msg msg)
{
    double speed  = msg.speed;
    int speed_int = (int)speed;
    int* data = &speed_int;
    constexpr float SPEED_OFFSET = 0.1f;  // floating reduction
    if (speed < SPEED_OFFSET)
        speed = 0.00f;
    m_can->send_frame(can_msg_id_t::GPS_ID, 4, data);
    mavlink_msg_gps_raw_int_send(MAVLINK_COMM_0, m_mav.microsSinceEpoch(), 3,
								 msg.Position_t.lat * 1e7,
								 msg.Position_t.lon * 1e7,
								 30000, //altitude
								 UINT16_MAX, // hdop
								 UINT16_MAX,
								 (msg.speed/3.6)*100, // speed
								 msg.cog*10.0, //cog
								 UINT8_MAX);
    m_mav.sensor_alive(MAV_SYS_STATUS_SENSOR_GPS);

    Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_3, COL_2, speed_int, "null");

}

void Gui::on_gps_fix_changed(bool is_fixed)
{
    QString txt = "GPS " + (is_fixed? "" : QString("NO ")) + "FIX";
    //TODO: gotta think about this
  /*  this->ui->lGpsFix->setText(txt);
    if (is_fixed)
        this->ui->signal_icon->setStyleSheet("border-image:url(:gps/fix.png) 2 5 2 5;");
    else
        this->ui->signal_icon->setStyleSheet("border-image:url(:gps/no_fix.png) 2 5 2 5;"); */
}

void Gui::on_can_frame_received(can_frame cf)
{
    char frame_buff[100];
    sprint_canframe(frame_buff, &cf, ' ');
    QString frame_str(frame_buff);
    // tmp log

    static int debug_var = 0;

    QStringList id_names = {
        "COOLING SYSTEM PARAMETERS",
        "PUMP MODE",
        "COOLING SYSTEM ERRORS",
        "MOTOR POWER",
        "HYDROFOIL ANGLE",
        "BILGE SYSTEM ERRORS",
        "IMU ANGLES",
        "IMU ACC",
        "BMS DATA",
        "SPEED",
        "GPS",
    };
    QStringList alarm_e {"OVERHEAT",  // too high motor temperature
                "DRY RUNNING",			// no water in cooling system
                "LEAK",					// detected leakage in cooling system
                "NONE",					// no error
                "TIMEOUT"};             // task failed to read cooling system parameters in specified time

    // log whole frame
    const int can_ID = cf.can_id;
    //const QString date = QDateTime::currentDateTime().toString("hh:mm:ss");
    const QString date = "[--]";
    // other id
    if (can_ID > 16 || can_ID < 6)
    {
        printf("Unknown message id");
        return;
    }

    const QString id_string = id_names[can_ID - CAN_ID_OFFSET];
    const QString log_data = date + QString(" [%1]").arg(can_ID, 2) + " - " + id_string;


    switch (can_ID) {
    case can_msg_id_t::COOLING_SYSTEM_PARAMETERS_ID:
    {
        can_pump_state_input psi;
        memcpy(&psi, cf.data, sizeof(can_pump_state_input));
        int mot1_temp = (int)(psi.motor_one_temp * 10);
        int mot2_temp = (int)(psi.motor_two_temp * 10);

        /****************** DRAW VALUES ON EPAPER **************/
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_1, COL_3, mot1_temp / 10, "deg");
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_2, COL_3, mot2_temp / 10, "deg");
        break;
    }

    case can_msg_id_t::PUMP_MODE_ID:
        // only to send - souldnt be received
        break;

    case can_msg_id_t::COOLING_SYSTEM_ERRORS_ID:
        // SBT_e_pump_alarm?
        // ui->tb_errors->append(date + " " + id_string + " -> " + alarm_e[cf.data[0]] + "!");
        break;

    case can_msg_id_t::MOTOR_POWER_ID:
    {
        can_motor_power mp;
        memcpy(&mp, cf.data, sizeof(can_motor_power));
        double throttle = mp.potentiometer_value1/20.f + 50;
        int int_throrrtle = (int)(throttle*100);

        /****************** DRAW VALUES ON EPAPER **************/
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_3, COL_1, int_throrrtle, "perc");
        break;
    }

    case can_msg_id_t::HYDROFOIL_ANGLE_ID:
        can_hydrofoil_angle ha;
        memcpy(&ha, cf.data, sizeof(can_hydrofoil_angle));
        break;

    case can_msg_id_t::BILGE_SYSTEM_ERRORS_ID:
        // ...
        // Coś z 'SBT_e_bilge_event' ?
        // ui->tb_errors->append(date + " " + id_string + " -> " /* + alarm_e[cf.data[0]] */ + "!");
        break;

    case can_msg_id_t::IMU_ANGLES_ID: {
        can_imu_angles iang;
        memcpy(&iang, cf.data, sizeof(iang));
        m_mav.sensor_alive(MAV_SYS_STATUS_AHRS);
        mavlink_msg_attitude_send(MAVLINK_COMM_0,
                                  get_program_ticks(),
                                  (iang.imu_roll /100.f) * DEG2RAD,
                                  (iang.imu_pitch /100.f) * DEG2RAD,
                                  (iang.imu_yaw /100.f) * DEG2RAD,
                                  0, 0, 0); // Angular speed
        break;
    }

    case can_msg_id_t::IMU_ACC_ID: {
        can_imu_acc iacc;
        memcpy(&iacc, cf.data, sizeof(can_imu_acc));
        m_mav.sensor_alive(MAV_SYS_STATUS_SENSOR_3D_ACCEL);
        mavlink_msg_scaled_imu_send(MAVLINK_COMM_0,
                                    get_program_ticks(),
                                    iacc.imu_acc_x /100.f,
                                    iacc.imu_acc_x /100.f,
                                    iacc.imu_acc_x /100.f,
                                    0, 0, 0, // angular speed
                                    0, 0, 0); // Magnetometer
        break;
    }

    case can_msg_id_t::BMS_DATA_ID:
    {
        can_bms_data bmsd;
        memcpy(&bmsd, cf.data, sizeof(can_bms_data));

        double min_vol = bmsd.max_cell_volatage / 10000.f;
        double max_vol = bmsd.min_cell_voltage / 10000.f;
        double discharge_cur = bmsd.discharge_cuttent / 100.f;
        double charge_cur = bmsd.charge_current/ 10000.f;
        
        int char_power = (int)(charge_cur * (min_vol + max_vol)/2);
        int dischar_power = (int)(discharge_cur * (min_vol + max_vol)/2);

        /****************** DRAW VALUES ON EPAPER **************/
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_1, COL_1, char_power, "watt");
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_2, COL_1, dischar_power, "watt");
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_1, COL_2, max_vol, "volt");
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_2, COL_2, min_vol, "volt");

	    m_mav.sensor_alive(MAV_SYS_STATUS_SENSOR_BATTERY);
        m_mav.update_battery_info((bmsd.min_cell_voltage * 12) / 10, bmsd.discharge_cuttent, -1);
        uint16_t cell_volts[10] = {
            (uint16_t) (bmsd.min_cell_voltage / 10),
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX,
            UINT16_MAX
        };
        mavlink_msg_battery_status_send(MAVLINK_COMM_0, 0,
                                        MAV_BATTERY_FUNCTION_ALL,
                                        MAV_BATTERY_TYPE_LION,
                                        INT16_MAX,       // [cDeg]temperature
                                        cell_volts, // [mV]  Cell voltages
                                        bmsd.discharge_cuttent,   // [cA]  Discharge Current
                                        -1,  // [mAh] Consumed charge
                                        bmsd.charge_current,       // [cA]  Charge current (custom by us)
                                        -1);        // [%]   Battery remaining
        break;
    }

    case can_msg_id_t::SPEED_ID:
        // ...
        break;
    case can_msg_id_t::GPS_ID:
        // ...
        break;
    default:
        Dynamic_Refresh(SIZE_X, SIZE_Y, ROW_3, COL_3, debug_var, "null");
        debug_var++;
        // ui->tb_logs->append(QString::number(can_ID) + " - not defined");
        break;
    }



}
