#ifndef GUI_HPP
#define GUI_HPP

#include <QString>
#include <QJsonObject>
#include <chrono>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <cstdio>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

#include "draw_gui/draw_gui.hpp"
#include "can/sbt_can_msg.h"
#include "can/utils.h"
#include "mav.hpp"
#include "mavlink/standard/mavlink.h"
#include "gps.hpp"
#include "can.hpp"
#include "mav.hpp"


constexpr char GPS_PORT[] = "/dev/ttyACM0";
constexpr char CAN_ITF[]  = "can0";

//QT_FORWARD_DECLARE_CLASS(QJsonArray)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)


QT_BEGIN_NAMESPACE
namespace Ui { class Gui; }
QT_END_NAMESPACE

class Gui : public QObject
{
    Q_OBJECT
private:
    Gps     *gps;
    CAN     *m_can;
    Mavlink m_mav;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;

    QList<QPushButton*> m_other_pomps_list;
    QList<QLabel*>      m_parameters_list;
    QJsonObject         m_parameters;

public:
    Gui();

private slots:
    void on_gps_data_received(sbt_RMC_msg msg);

    void on_gps_fix_changed(bool is_fixed);

    void on_can_frame_received(can_frame cf);

    uint32_t get_program_ticks() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_start_time).count();
    }
};

#endif // GUI_HPP
