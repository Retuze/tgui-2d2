#include "../include/tgui_constraint.h"
#include "../include/tgui_widget.h"
#include <stdlib.h>
#include <math.h>

// 获取控件的边缘位置
static int16_t get_edge_position(const tgui_widget_t* widget, tgui_edge_t edge) {
    if (widget == NULL) return 0;
    
    switch (edge) {
        case TGUI_EDGE_LEFT:
            return widget->x;
        case TGUI_EDGE_RIGHT:
            return widget->x + widget->width;
        case TGUI_EDGE_TOP:
            return widget->y;
        case TGUI_EDGE_BOTTOM:
            return widget->y + widget->height;
        case TGUI_EDGE_CENTER_X:
            return widget->x + widget->width / 2;
        case TGUI_EDGE_CENTER_Y:
            return widget->y + widget->height / 2;
        default:
            return 0;
    }
}

// 设置控件的边缘位置
static void set_edge_position(tgui_widget_t* widget, tgui_edge_t edge, int16_t position) {
    if (widget == NULL) return;
    
    switch (edge) {
        case TGUI_EDGE_LEFT:
            widget->x = position;
            break;
        case TGUI_EDGE_RIGHT:
            widget->x = position - widget->width;
            break;
        case TGUI_EDGE_TOP:
            widget->y = position;
            break;
        case TGUI_EDGE_BOTTOM:
            widget->y = position - widget->height;
            break;
        case TGUI_EDGE_CENTER_X:
            widget->x = position - widget->width / 2;
            break;
        case TGUI_EDGE_CENTER_Y:
            widget->y = position - widget->height / 2;
            break;
    }
}

void tgui_constraint_solver_init(tgui_constraint_solver_t* solver) {
    if (solver == NULL) return;
    
    solver->constraints = NULL;
    solver->count = 0;
}

void tgui_constraint_solver_add(tgui_constraint_solver_t* solver, 
                              const tgui_constraint_t* constraint) {
    if (solver == NULL || constraint == NULL) return;
    
    // 重新分配内存
    tgui_constraint_t* new_constraints = (tgui_constraint_t*)realloc(
        solver->constraints, 
        (solver->count + 1) * sizeof(tgui_constraint_t));
    if (new_constraints == NULL) return;
    
    solver->constraints = new_constraints;
    solver->constraints[solver->count++] = *constraint;
}

tgui_result_t tgui_constraint_solver_add_constraint(tgui_constraint_solver_t* solver, tgui_widget_t* widget, tgui_constraint_t* constraint) {
    if (solver->constraint_count >= TGUI_MAX_CONSTRAINTS) {
        return TGUI_ERR_BUFFER_FULL;
    }

    // 设置约束的self_id为当前widget的id
    constraint->self_id = widget->id;
    
    // 复制约束到solver中
    solver->constraints[solver->constraint_count] = *constraint;
    solver->constraint_count++;

    return TGUI_OK;
}

static void apply_constraint(tgui_widget_t* widget, 
                           const tgui_constraint_t* constraint,
                           tgui_widget_t* target) {
    if (widget == NULL || constraint == NULL) return;
    
    int16_t target_pos = target ? get_edge_position(target, constraint->target_edge) : 0;
    int16_t new_pos;
    
    switch (constraint->type) {
        case TGUI_CONSTRAINT_FIXED:
            new_pos = target_pos + constraint->value.fixed_value;
            set_edge_position(widget, constraint->self_edge, new_pos);
            break;
            
        case TGUI_CONSTRAINT_FLEXIBLE:
            {
                int16_t min_pos = target_pos + constraint->value.flex.min_value;
                int16_t max_pos = target_pos + constraint->value.flex.max_value;
                int16_t current_pos = get_edge_position(widget, constraint->self_edge);
                
                // 使用权重在min和max之间插值
                new_pos = min_pos + (int16_t)((max_pos - min_pos) * constraint->value.flex.weight);
                new_pos = new_pos < min_pos ? min_pos : (new_pos > max_pos ? max_pos : new_pos);
                set_edge_position(widget, constraint->self_edge, new_pos);
            }
            break;
            
        case TGUI_CONSTRAINT_PERCENT:
            if (target) {
                int16_t target_size = (constraint->target_edge == TGUI_EDGE_LEFT || 
                                     constraint->target_edge == TGUI_EDGE_RIGHT) ?
                                    target->width : target->height;
                new_pos = target_pos + (int16_t)(target_size * constraint->value.percent);
                set_edge_position(widget, constraint->self_edge, new_pos);
            }
            break;
            
        case TGUI_CONSTRAINT_ASPECT:
            if (constraint->self_edge == TGUI_EDGE_RIGHT || 
                constraint->self_edge == TGUI_EDGE_BOTTOM) {
                int16_t size = (constraint->self_edge == TGUI_EDGE_RIGHT) ?
                              widget->height : widget->width;
                new_pos = get_edge_position(widget, 
                    (constraint->self_edge == TGUI_EDGE_RIGHT) ? 
                    TGUI_EDGE_LEFT : TGUI_EDGE_TOP);
                new_pos += (int16_t)(size * constraint->value.aspect_ratio);
                set_edge_position(widget, constraint->self_edge, new_pos);
            }
            break;
    }
    
    // 处理角度约束
    if (constraint->angle_type != TGUI_ANGLE_NONE) {
        // TODO: 实现角度约束
    }
}

// 获取控件树中指定ID的控件
static tgui_widget_t* find_widget_by_id(tgui_widget_t* root, tgui_id_t id) {
    if (!root) return NULL;
    if (root->id == id) return root;
    
    // 在子控件中查找
    for (tgui_widget_t* child = root->children; child != NULL; child = child->next) {
        tgui_widget_t* result = find_widget_by_id(child, id);
        if (result) return result;
    }
    
    return NULL;
}

void tgui_constraint_solver_solve(tgui_constraint_solver_t* solver,
                                tgui_widget_t* root,
                                uint16_t widget_count) {
    if (solver == NULL || root == NULL) return;
    
    // 遍历所有约束
    for (uint16_t i = 0; i < solver->count; i++) {
        tgui_constraint_t* c = &solver->constraints[i];
        
        // 查找目标控件和源控件
        tgui_widget_t* target = find_widget_by_id(root, c->target_id);
        tgui_widget_t* self = find_widget_by_id(root, c->self_id);  // 使用self_id查找自身控件
        
        if (target && self) {
            apply_constraint(self, c, target);
        }
    }
}