#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <raylib.h>
#include <stdlib.h>

typedef struct
{
  Vector2 position;
  Vector2 velocity;
  float radius;
  // float mass;
  Color color;
} Particle;

#define N 1000

int
main (void)
{
  int screenWidth = 600 * 2;
  int screenHeight = 600 * 2;
  const gsl_rng *r;
  const gsl_rng_type *t;
  gsl_rng_env_setup ();
  t = gsl_rng_default;
  r = gsl_rng_alloc (t);

  Particle ensemble[N];
  int i;

  for (i = 0; i < N; i++)
    {
      ensemble[i].position.x
          = gsl_ran_gaussian (r, 50) + (float)screenWidth / 2 + 50;
      ensemble[i].position.y
          = gsl_ran_gaussian (r, 50) + (float)screenHeight / 2 + 50;
      ensemble[i].velocity.x = gsl_ran_gaussian (r, 5);
      ensemble[i].velocity.y = gsl_ran_gaussian (r, 5);
      ensemble[i].radius = 8.0;
      ensemble[i].color = RAYWHITE;
    }

  for (i = 0; i < N / 10; i++)
    {
      ensemble[i].color = MAROON;
    }

  InitWindow (screenWidth, screenHeight, "Gas Box");
  SetTargetFPS (60);

  while (!WindowShouldClose ())
    {
      BeginDrawing ();
      ClearBackground (BLACK);
      for (i = 0; i < N; i++)
        {
          ensemble[i].position.x += ensemble[i].velocity.x;
          ensemble[i].position.y += ensemble[i].velocity.y;

          if (ensemble[i].position.x - ensemble[i].radius <= 0
              || ensemble[i].position.x + ensemble[i].radius >= screenWidth)
            ensemble[i].velocity.x = -1.0 * ensemble[i].velocity.x;

          if (ensemble[i].position.y - ensemble[i].radius <= 0
              || ensemble[i].position.y + ensemble[i].radius >= screenHeight)
            ensemble[i].velocity.y = -1.0 * ensemble[i].velocity.y;

          DrawCircleV (ensemble[i].position, ensemble[i].radius,
                       ensemble[i].color);
        }
      EndDrawing ();
    }
  CloseWindow ();

  exit (EXIT_SUCCESS);
}
