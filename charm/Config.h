#ifndef CONFIG_H
#define CONFIG_H

#include "config/config.h"

EMP_BUILD_CONFIG(Config,
  GROUP(WORLD_STRUCTURE, "How should the world be setup?"),
  VALUE(GRID_WIDTH, size_t, 24, "How wide is the World?"),
  VALUE(GRID_HEIGHT, size_t, 24, "How tall is the World? (should equal GRID_WIDTH)"),

  GROUP(RUN_STRUCTURE, "How should the run be setup?"),
  VALUE(RUN_DURATION, double, 150.0, "How long should a run last (seconds)?"),
  VALUE(WAVE_DURATION, double, 5.0, "At most, how long should a wave last (seconds)?"),

  GROUP(RESOURCE_STRUCTURE, "How should resources be setup?"),
  VALUE(WAVE_SIZE, double, 3.0, "What radius should resource waves be good in?"),
  VALUE(WAVE_REWARD, double, 1.0, "How much resource to gain on good resource wave?"),
  VALUE(WAVE_PENALTY, double, -5.0, "How much resource to lose on bad resource wave?")
)
#endif
