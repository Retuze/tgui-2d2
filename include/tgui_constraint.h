#ifndef TGUI_CONSTRAINT_H
#define TGUI_CONSTRAINT_H

#include <stdint.h>
#include "tgui_result.h"
#include "tgui_types.h"  // Include for tgui_edge_t

#define TGUI_MAX_CONSTRAINTS 100

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
struct tgui_widget;
typedef struct tgui_widget tgui_widget_t;

// Constraint value types
typedef enum {
    TGUI_CONSTRAINT_FIXED,
    TGUI_CONSTRAINT_FLEXIBLE,
    TGUI_CONSTRAINT_PERCENT,
    TGUI_CONSTRAINT_ASPECT
} tgui_constraint_value_type_t;

// Constraint structure
typedef struct {
    uint16_t target_id;  // ID of target widget
    uint16_t self_id;    // ID of self widget
    tgui_edge_t target_edge;
    tgui_edge_t self_edge;
    tgui_constraint_value_type_t type;
    union {
        int16_t fixed_value;
        struct {
            int16_t min_value;
            int16_t max_value;
            float weight;
        } flex;
        float percent;
        float aspect_ratio;
    } value;
} tgui_constraint_t;

// Constraint solver structure
typedef struct {
    tgui_constraint_t* constraints;
    int count;
    int capacity;
} tgui_constraint_solver_t;

// Function prototypes
void tgui_constraint_solver_init(tgui_constraint_solver_t* solver);
void tgui_constraint_solver_add(tgui_constraint_solver_t* solver, const tgui_constraint_t* constraint);
void tgui_constraint_solver_solve(tgui_constraint_solver_t* solver, tgui_widget_t* root_widget, uint16_t widget_count);
void tgui_constraint_solver_cleanup(tgui_constraint_solver_t* solver);

#ifdef __cplusplus
}
#endif

#endif // TGUI_CONSTRAINT_H