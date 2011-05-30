/**
 * cli.c - CLI class.
 *
 * Copyright (c) 2011, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the CLI.
 *
 * License:
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "render.h"
#include "scene.h"
#include "output.h"

#include "cli.h"

static char* cli_pop_token (char* line)
{
   return strtok (line, " ");
}

static void cli_enter_camera (void)
{
   char* line;
   char* token;
   int end = 0;
   int i;
   int param[3]; /* x, y, z or r, g, b */
   camera_t *cam = scene_get_camera ();

   while (!end)
   {
      line = readline ("scene/camera> ");

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "position"))
      {
         for (i=0; i<3; i++)
         {
            token = cli_pop_token (NULL);
            if (!token)
               continue;
            param[i] = strtol (token, NULL, 10);
         }
         cam->pos.x = param[0];
         cam->pos.y = param[1];
         cam->pos.z = param[2];
      }
      else
      if (!strcmp (token, "show"))
      {
         printf ("  x:%.0f, y:%.0f, z:%.0f\n",
                 cam->pos.x, cam->pos.y, cam->pos.z);
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("position <X> <Y> <Z>" "\tCamera position.\n");
         printf ("show"                 "\t\t\tShow camera settings.\n");
         printf ("help"                 "\t\t\tShow this help text.\n");
         printf ("end"                  "\t\t\tExit context.\n");
      }
      else
      if (!strcmp (token, "end"))
      {
         return;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

static void cli_enter_sphere (int id)
{
   char prompt[32];
   char* line;
   char* token;
   int end = 0;
   int i;
   int param[3]; /* x, y, z or r, g, b */
   sphere_t *sphere = scene_get_sphere ();

   while (!end)
   {
      snprintf (prompt, sizeof(prompt), "scene/sphere-%d> ", id);
      line = readline (prompt);

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "center"))
      {
         for (i=0; i<3; i++)
         {
            token = cli_pop_token (NULL);
            if (!token)
               continue;
            param[i] = strtol (token, NULL, 10);
         }
         sphere[id].center.x = param[0];
         sphere[id].center.y = param[1];
         sphere[id].center.z = param[2];
      }
      else
      if (!strcmp (token, "radius"))
      {
         token = cli_pop_token (NULL);
         if (!token)
            continue;
         sphere[id].radius = strtol (token, NULL, 10);
      }
      else
      if (!strcmp (token, "color"))
      {
         for (i=0; i<3; i++)
         {
            token = cli_pop_token (NULL);
            if (!token)
               continue;
            param[i] = strtol (token, NULL, 10);
         }
         sphere_set_color (&sphere[id], param[0], param[1], param[2]);
      }
      else
      if (!strcmp (token, "show"))
      {
         printf ("x:%.0f, y:%.0f, z:%.0f\n",
                 sphere[id].center.x, sphere[id].center.y, sphere[id].center.z);
         printf ("radius: %.0f\n", sphere[id].radius);
         printf ("r:%d, g:%d, b:%d\n", sphere[id].r, sphere[id].g, sphere[id].b);
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("center <X> <Y> <Z>"  "\tSphere center coordinates.\n");
         printf ("radius <R>"          "\t\tSphere radius\n");
         printf ("color <R> <G> <B>"   "\tSphere color\n");
         printf ("show"                "\t\t\tShow sphere settings.\n");
         printf ("help"                "\t\t\tShow this help text.\n");
         printf ("end"                 "\t\t\tExit context.\n");
      }
      else
      if (!strcmp (token, "end"))
      {
         return;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

static void cli_enter_scene (void)
{
   char* line;
   char* token;
   int end = 0;

   while (!end)
   {
      line = readline ("scene> ");

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "camera"))
      {
         printf ("Entering camera context\n");
         cli_enter_camera ();
      }
      else
      if (!strcmp (token, "sphere"))
      {
         int id;

         token = cli_pop_token (NULL);
         if (!token)
         {
            printf ("Missing sphere ID.\n");
            continue;
         }

         id = strtol (token, NULL, 10);
         if (id >=0  && id < scene_get_num_spheres())
         {
            printf ("Entering sphere context\n");
            cli_enter_sphere (id);
         }
         else
         {
            printf ("Invalid ID, must be between 0 and %d\n", scene_get_num_spheres());
         }
      }
      else
      if (!strcmp (token, "show"))
      {
         camera_t *cam = scene_get_camera ();
         sphere_t *sphere = scene_get_sphere ();
         int i;

         printf ("Camera\n");
         printf ("  x:%.0f, y:%.0f, z:%.0f\n",
                 cam->pos.x, cam->pos.y, cam->pos.z);
         for (i=0; i<scene_get_num_spheres (); i++)
         {
            printf ("Sphere %d\n", i);
            printf ("  x:%.0f, y:%.0f, z:%.0f\n",
                    sphere[i].center.x, sphere[i].center.y, sphere[i].center.z);
            printf ("  radius: %.0f\n", sphere[i].radius);
            printf ("  r:%d, g:%d, b:%d\n", sphere[i].r, sphere[i].g, sphere[i].b);
         }
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("camera"       "\t\tSetup camera.\n");
         printf ("sphere <ID>"  "\tSetup sphere object.\n");
         printf (               "\t\t<ID> sphere identity, 0-%d.\n", scene_get_num_spheres());
         printf ("show"         "\t\tShow objects settings.\n");
         printf ("help"         "\t\tShow this help text.\n");
         printf ("end"          "\t\tExit context.\n");
      }
      else
      if (!strcmp (token, "end"))
      {
         return;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

void cli_enter (void)
{
   char* line;
   char* token;
   int quit = 0;

   /* Init scene */
   scene_init ();

   printf ("Enter 'help' for available commands.\n");
   while (!quit)
   {
      line = readline ("> ");

      token = cli_pop_token (line);

      if (!token)
         continue;

      if (!strcmp (token, "scene"))
      {
         printf ("Entering scene context\n");
         cli_enter_scene ();
      }
      else
      if (!strcmp (token, "render"))
      {
         printf ("Rendering scene\n");
         /* Render the scene */
         if (render_scene (output_get_image (),
                           output_get_image_size (),
                           output_get_image_width (),
                           output_get_image_height (),
                           scene_get_camera (),
                           scene_get_sphere (),
                           scene_get_num_spheres ()))
            printf ("An error occured when rendering the scene.\n");
      }
      else
      if (!strcmp (token, "output"))
      {
         printf ("Calling rendering output function\n");
         if (output_render ())
            printf ("An error occured when calling the rendering output function.\n");
      }
      else
      if (!strcmp (token, "help"))
      {
         printf ("scene"   "\tEnter scene context.\n");
         printf ("render"  "\tRender scene.\n");
         printf ("output"  "\tSend the rendered scene to output function.\n");
         printf ("help"    "\tShow this help text.\n");
         printf ("quit"    "\tQuit.\n");
      }
      else
      if (!strcmp (token, "quit"))
      {
         quit = 1;
      }
      else
      {
         if (strlen (token))
            printf ("Unknown command\n");
      }
   }
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
