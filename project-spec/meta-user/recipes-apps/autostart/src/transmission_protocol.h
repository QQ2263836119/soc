#ifndef TRANSMISSION_PROTOCOL_H
#define TRANSMISSION_PROTOCOL_H 1
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
// https://stackoverflow.com/a/48521433/16027269
#define termios asmtermios
#include <asm/termios.h>
#undef termios
#include <termios.h>

#include "coding.h"

__BEGIN_DECLS

#define TP_BAUD_RATE B500000
#define TP_FRAME_DATA_LEN_MAX 512

enum {
  TP_STATUS_NETWORK_ENCODING = 0x01,
  TP_STATUS_ENTROPY_ENCODING = 0x02,
};
enum {
  TP_ADDRESS_MASTER,
  TP_ADDRESS_SLAVE,
};
enum {
  TP_FRAME_TYPE_QUERY = 1,
  TP_FRAME_TYPE_SEND = 3,
  TP_FRAME_TYPE_RECV = 7,
};
enum {
  TP_STATUS_UNRECEIVED = 0xAAAA,
  TP_STATUS_PROCESSING = 0xAA55,
  TP_STATUS_PROCESSED = 0x5555,
};
enum {
  TP_FLAG_1_H265 = 0x8A,
  TP_FLAG_1_JPEG = 0x8C,
  TP_FLAG_1_YUV420 = 0x8E,
  TP_FLAG_2_H265 = 0x9A,
  TP_FLAG_2_JPEG = 0x9C,
  TP_FLAG_2_YUV420 = 0x9E,
  TP_FLAG_DATA = 0xAA,
};
typedef uint8_t address_t;
typedef uint8_t frame_type_t;
typedef uint32_t n_file_t;
typedef uint16_t n_frame_t;
typedef uint16_t data_len_t;
typedef uint16_t status_t;
typedef struct {
  address_t address;
  frame_type_t frame_type;
  n_file_t n_file;
  n_frame_t n_frame;
  status_t status;
  uint16_t check_sum;
} __attribute__((packed)) frame_t;
// https://www.reddit.com/r/C_Programming/comments/416li3/24_bit_unsigned_int/
typedef struct {
  unsigned uint24 : 24;
} __attribute__((packed)) uint24_t;
typedef uint24_t n_total_frame_t;
typedef uint8_t flag_t;
typedef uint32_t total_data_len_t;
typedef struct {
  uint8_t header[4];
  n_total_frame_t n_total_frame;
  flag_t flag;
  n_file_t n_file;
  n_frame_t n_frame;
  total_data_len_t total_data_len;
  data_len_t data_len;
  uint8_t data[TP_FRAME_DATA_LEN_MAX];
  uint16_t check_sum;
} __attribute__((packed)) data_frame_t;

char *bin_to_str(uint8_t const *bin, size_t size);
n_frame_t id_to_n_frame(n_frame_t id, n_frame_t len);
n_frame_t n_frame_to_id(n_frame_t n_frame, n_frame_t len);
n_frame_t count_unreceived_data_frames(data_frame_t *, n_frame_t);
struct termios init_tty(int fd);

ssize_t write_frame(int fd, const frame_t *frame);
ssize_t send_frame(int, const frame_t *, int);
ssize_t receive_frame(int, frame_t *, int);
ssize_t write_data_frame(int fd, const data_frame_t *frame);
ssize_t send_data_frame(int, data_frame_t *, int);
n_frame_t receive_data_frames(int recv_fd, data_frame_t *input_data_frames,
                              frame_t input_frame, int timeout);
ssize_t send_data_frame_directly(int fd, const data_frame_t *frame,
                                 int timeout);

void init_data_frames(data_frame_t *, n_frame_t, n_file_t, flag_t,
                      total_data_len_t);
data_frame_t *alloc_data_frames(n_frame_t, n_file_t, uint8_t *, size_t, flag_t,
                                total_data_len_t);
int get_data_frames(char *filename, n_file_t *const n_file,
                    n_frame_t *const n_frame,
                    data_frame_t **p_output_data_frames, bool binary);
size_t data_frame_to_data_len(data_frame_t *, n_frame_t);
void data_frames_to_data(data_frame_t *, n_frame_t, uint8_t *);
void data_to_data_frames(uint8_t *, size_t, data_frame_t *);
void fd_to_data_frames(int, data_frame_t *, n_frame_t);
void entropy_to_gmm(int16_t *, gmm_t *, size_t);

__END_DECLS
#endif /* transmission_protocol.h */
