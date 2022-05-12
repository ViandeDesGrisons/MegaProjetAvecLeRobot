#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"

#define SOUND		0
#define OBSTACLE	1

/** Robot wide IPC bus. */
extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

//extern struct Phases phase;


#ifdef __cplusplus
}
#endif

#endif
