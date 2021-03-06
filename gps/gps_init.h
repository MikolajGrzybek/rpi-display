#ifndef GPS_INIT_H_
#define GPS_INIT_H_

// Structures
struct gps_init_msg {
    int     m_len;
    uint8_t *m_buff;
};
typedef struct gps_init_msg gps_init_msg;

struct gps_init_cmd {
    int          m_num;
    gps_init_msg *m_messages;
};
typedef struct gps_init_cmd gps_init_cmd;

// Utility macros
#define BUFF_LEN(x) sizeof((x))/sizeof((x)[0])
#define _ID(x) (x)
#define _CMD_MSG(cmd, msg_num) cmd ## _msg_ ## msg_num
#define CMD_MSG(cmd, msg_num) _CMD_MSG(cmd, msg_num)
#define _CMD_MSG_BUFF(cmd, msg_num) cmd ## _msg_ ## msg_num ## _buff
#define CMD_MSG_BUFF(cmd, msg_num) _CMD_MSG_BUFF(cmd, msg_num)
#define CMD_MSG_LEN(cmd, msg_num) BUFF_LEN(CMD_MSG_BUFF(cmd, msg_num))
#define _CMD_MSG_ARRAY(cmd) cmd ## _array
#define CMD_MSG_ARRAY(cmd) _CMD_MSG_ARRAY(cmd)

#define DEFINE_MSG(cmd, msg_num) \
    uint8_t CMD_MSG_BUFF(cmd, msg_num)[] = cmd ## _MSG_ ## msg_num ## _BUFF; \
    const gps_init_msg CMD_MSG(cmd, msg_num) = { \
        CMD_MSG_LEN(cmd, msg_num), \
        CMD_MSG_BUFF(cmd, msg_num) \
    };
#define DECLARE_MSG_ARRAY(cmd, msg_num) static gps_init_msg CMD_MSG_ARRAY(cmd) [ msg_num ];
#define DECLARE_CMD_ARRAY(cmd, cmd_num) static gps_init_cmd cmd

// Definitions of init commands
#define INIT_CMD_00 discard_GxGGA
#define INIT_CMD_00_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x24 }
#define INIT_CMD_00_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x00, 0xF9, 0x11 }
#define INIT_CMD_00_MSG_2_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }
#define INIT_CMD_00_MSG_3_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x24 }
#define INIT_CMD_00_MSG_4_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }

#define INIT_CMD_01 discard_GxGGL
#define INIT_CMD_01_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B }
#define INIT_CMD_01_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x01, 0xFA, 0x12 }
#define INIT_CMD_01_MSG_2_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }
#define INIT_CMD_01_MSG_3_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B }
#define INIT_CMD_01_MSG_4_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }

#define INIT_CMD_02 discard_GxGSA
#define INIT_CMD_02_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32 }
#define INIT_CMD_02_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x02, 0xFB, 0x13 }
#define INIT_CMD_02_MSG_2_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }
#define INIT_CMD_02_MSG_3_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32 }
#define INIT_CMD_02_MSG_4_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }

#define INIT_CMD_03 discard_GxGSV
#define INIT_CMD_03_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39 }
#define INIT_CMD_03_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x03, 0xFC, 0x14 }
#define INIT_CMD_03_MSG_2_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }
#define INIT_CMD_03_MSG_3_BUFF { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39 }
#define INIT_CMD_03_MSG_4_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38 }

#define INIT_CMD_04 rate_10Hz
#define INIT_CMD_04_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12 }
#define INIT_CMD_04_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30 }
#define INIT_CMD_04_MSG_2_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x08, 0x16, 0x3F }
#define INIT_CMD_04_MSG_3_BUFF { 0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12 }
#define INIT_CMD_04_MSG_4_BUFF { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x08, 0x16, 0x3F }

#define INIT_CMD_05 baud_115200
#define INIT_CMD_05_MSG_0_BUFF { 0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x7E }
#define INIT_CMD_05_MSG_1_BUFF { 0xB5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x01, 0x08, 0x22 }

#define INIT_CMD_NUM 6
#endif /* GPS_INIT_H_ */