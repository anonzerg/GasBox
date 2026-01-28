#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define N 1000
#define G 0.1

typedef struct
{
  Vector2 position;
  Vector2 velocity;
  float radius;
  // float mass;
  Color color;
} Particle;

void InitParticle (Particle *particle, Vector2 position, Vector2 velocity,
                   float radius, Color color);
void CollisionWithWall (Particle *particle, int screenWidth, int screenHeight);
void ImpulseCollision (Particle *particle1, Particle *particle2);
void Gravity (Particle *particle1, Particle *particle2);

int
main (void)
{
  const int screenWidth = 600 * 2;
  const int screenHeight = 600 * 2;
  const int fps = 120;

  const gsl_rng *r;
  const gsl_rng_type *t;
  gsl_rng_env_setup ();
  t = gsl_rng_default;
  r = gsl_rng_alloc (t);

  Particle ensemble[N];
  const float radius = 8.0f;
  const float speed = 40.0f;

  int i, j;
  for (i = 0; i < N; i++)
    {
      bool validPosition = false;
      Vector2 position;

      while (!validPosition)
        {
          position = (Vector2){
            gsl_ran_gaussian (r, 100.0) + (float)screenWidth / 2,
            gsl_ran_gaussian (r, 100.0) + (float)screenHeight / 2
          };
          validPosition = true;
          for (j = 0; j < i; j++)
            {
              float dist = Vector2Distance (position, ensemble[j].position);
              if (dist < 2 * radius)
                {
                  validPosition = false;
                  break;
                }
            }
        }

      Vector2 velocity = (Vector2){ gsl_ran_rayleigh (r, speed),
                                    gsl_ran_rayleigh (r, speed) };
      InitParticle (&ensemble[i], position, velocity, radius, RAYWHITE);
    }

  InitWindow (screenWidth, screenHeight, "Simulation of Gas in Box");
  SetTargetFPS (fps);

  while (!WindowShouldClose ())
    {
      BeginDrawing ();
      ClearBackground (DARKGRAY);
      for (i = 0; i < N; i++)
        {
          CollisionWithWall (&ensemble[i], screenWidth, screenHeight);
          ensemble[i].position = Vector2Add (
              ensemble[i].position,
              Vector2Scale (ensemble[i].velocity, GetFrameTime ()));
        }

      for (i = 0; i < N; i++)
        {
          for (j = i + 1; j < N; j++)
            {
              Gravity (&ensemble[i], &ensemble[j]);
              float dist = Vector2Distance (ensemble[i].position,
                                            ensemble[j].position);
              if (dist <= ensemble[i].radius + ensemble[j].radius)
                {
                  ImpulseCollision (&ensemble[i], &ensemble[j]);
                }
            }
        }

      for (i = 0; i < N; i++)
        {
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
  // left wall
  if (particle->position.x - particle->radius <= 0)
    {
      particle->position.x = particle->radius;
      particle->velocity.x = fabs (particle->velocity.x);
    }
  // right wall
  else if (particle->position.x + particle->radius >= screenWidth)
    {
      particle->position.x = screenWidth - particle->radius;
      particle->velocity.x = -fabs (particle->velocity.x);
    }
  // top wall
  if (particle->position.y - particle->radius <= 0)
    {
      particle->position.y = particle->radius;
      particle->velocity.y = fabs (particle->velocity.y);
    }
  // bottom wall
  else if (particle->position.y + particle->radius >= screenHeight)
    {
      particle->position.y = screenHeight - particle->radius;
      particle->velocity.y = -fabs (particle->velocity.y);
    }
}

void
ImpulseCollision (Particle *particle1, Particle *particle2)
{
  Vector2 normal = Vector2Normalize (
      Vector2Subtract (particle2->position, particle1->position));
  float relativeVelocity = Vector2DotProduct (
      Vector2Subtract (particle1->velocity, particle2->velocity), normal);

  if (relativeVelocity <= 0)
    return;

  float impulse = relativeVelocity;
  Vector2 impulseVector = Vector2Scale (normal, impulse);

  particle1->velocity = Vector2Subtract (particle1->velocity, impulseVector);
  particle2->velocity = Vector2Add (particle2->velocity, impulseVector);
}

void
Gravity (Particle *particle1, Particle *particle2)
{
  Vector2 deltaPosition
      = Vector2Subtract (particle2->position, particle1->position);

  float force = G / Vector2LengthSqr (deltaPosition);
  Vector2 direction = Vector2Normalize (deltaPosition);
  Vector2 acceleration = Vector2Scale (direction, force);

  particle1->velocity = Vector2Add (particle1->velocity, acceleration);
  particle2->velocity = Vector2Subtract (particle2->velocity, acceleration);
}
