#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <raylib.h>
#include <raymath.h>
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

void InitParticle (Particle *particle, Vector2 position, Vector2 velocity,
                   float radius, Color color);
void CollisionWithWall (Particle *particle, int screenWidth, int screenHeight);

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
      Vector2 position
          = (Vector2){ gsl_ran_gaussian (r, 50.0) + (float)screenWidth / 2,
                       gsl_ran_gaussian (r, 50.0) + (float)screenHeight / 2 };
      Vector2 velocity
          = (Vector2){ gsl_ran_rayleigh (r, 5.0), gsl_ran_rayleigh (r, 5.0) };
      InitParticle (&ensemble[i], position, velocity, 8.0, RAYWHITE);
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
          ensemble[i].position
              = Vector2Add (ensemble[i].position, ensemble[i].velocity);
          CollisionWithWall (&ensemble[i], screenWidth, screenHeight);

          DrawCircleV (ensemble[i].position, ensemble[i].radius,
                       ensemble[i].color);
        }
      EndDrawing ();
    }
  CloseWindow ();

  exit (EXIT_SUCCESS);
}

void
InitParticle (Particle *particle, Vector2 position, Vector2 velocity,
              float radius, Color color)
{
  particle->position = position;
  particle->velocity = velocity;
  particle->radius = radius;
  particle->color = color;
}

void
CollisionWithWall (Particle *particle, int screenWidth, int screenHeight)
{
  if (particle->position.x - particle->radius <= 0
      || particle->position.x + particle->radius >= screenWidth)
    particle->velocity.x = -1.0 * particle->velocity.x;

  if (particle->position.y - particle->radius <= 0
      || particle->position.y + particle->radius >= screenHeight)
    particle->velocity.y = -1.0 * particle->velocity.y;
}
