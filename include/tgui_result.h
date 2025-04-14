#ifndef TGUI_RESULT_H
#define TGUI_RESULT_H

#ifdef __cplusplus
extern "C" {
#endif

// Result type
typedef int tgui_result_t;

// Result codes
#define TGUI_OK                  0
#define TGUI_ERR_BUFFER_FULL    -1
#define TGUI_ERR_INVALID_PARAM  -2
#define TGUI_ERR_NOT_FOUND      -3
#define TGUI_ERR_FAILED         -4

#ifdef __cplusplus
}
#endif

#endif // TGUI_RESULT_H 