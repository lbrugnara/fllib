#ifndef FL_TIMER_H
#define FL_TIMER_H

#include "../Types.h"

typedef struct FlTimer* FlTimer;

FlTimer fl_timer_create();
void fl_timer_start(FlTimer timer);
void fl_timer_end(FlTimer timer);
long fl_timer_elapsed_ms(FlTimer timer);
void fl_timer_free(FlTimer timer);

#endif /* FL_TIMER_H */
