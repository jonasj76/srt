/**
 * xml.c - XML class.
 *
 * Copyright (c) 2013, Jonas Johansson <jonasj76@gmail.com>
 *
 * Description:
 * This class defines the XML functions.
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

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "camera.h"
#include "sphere.h"
#include "scene.h"

#include "xml.h"

#define TAG_SCENE  (const xmlChar*)"scene"
#define TAG_CAMERA (const xmlChar*)"camera"
#define TAG_SPHERE (const xmlChar*)"sphere"
#define TAG_ID     (const xmlChar*)"id"
#define TAG_DATA   (const xmlChar*)"data"
#define TAG_X      (const xmlChar*)"x"
#define TAG_Y      (const xmlChar*)"y"
#define TAG_Z      (const xmlChar*)"z"
#define TAG_FOV    (const xmlChar*)"fov"
#define TAG_RADIUS (const xmlChar*)"radius"
#define TAG_R      (const xmlChar*)"r"
#define TAG_G      (const xmlChar*)"g"
#define TAG_B      (const xmlChar*)"b"

/**
 * parse_sphere - Parse sphere object.
 * @cur: xml node
 * @id: Sphere id
 *
 * Returns:
 * none.
 */
void parse_sphere (xmlNodePtr cur, int id)
{
   sphere_t* sphere = scene_get_sphere (scene_get_scene ());
   xmlChar *prop;

   if (id<0 || id>scene_get_num_spheres ())
   {
      printf("Invalid sphere ID (%d), skipping...", id);
      return;
   }

   while (cur)
   {
      if (!xmlStrcmp (cur->name, TAG_DATA))
      {
         // Center
         prop = xmlGetProp (cur, TAG_X);
         if (prop)
            sphere[id].center.x = atof((char*)prop);
         prop = xmlGetProp (cur, TAG_Y);
         if (prop)
            sphere[id].center.y = atof((char*)prop);
         prop = xmlGetProp (cur, TAG_Z);
         if (prop)
            sphere[id].center.z = atof((char*)prop);
         // Radius
         prop = xmlGetProp(cur, TAG_RADIUS);
         if (prop)
            sphere[id].radius = atof((char*)prop);
         // Color
         prop = xmlGetProp (cur, TAG_R);
         if (prop)
            sphere[id].r = atoi((char*)prop);
         prop = xmlGetProp (cur, TAG_G);
         if (prop)
            sphere[id].g = atoi((char*)prop);
         prop = xmlGetProp (cur, TAG_B);
         if (prop)
            sphere[id].b = atoi((char*)prop);
      }
      cur = cur->next;
   }
}

/**
 * parse_camera - Parse camera object.
 * @cur: xml node
 *
 * Returns:
 * none.
 */
void parse_camera (xmlNodePtr cur)
{
   camera_t* cam = scene_get_camera (scene_get_scene ());
   xmlChar *prop;

   while (cur)
   {
      if (!xmlStrcmp (cur->name, TAG_DATA))
      {
         // Position
         prop = xmlGetProp (cur, TAG_X);
         if (prop)
            cam->pos.x = atof((char*)prop);
         prop = xmlGetProp (cur, TAG_Y);
         if (prop)
            cam->pos.y = atof((char*)prop);
         prop = xmlGetProp (cur, TAG_Z);
         if (prop)
            cam->pos.z = atof((char*)prop);
         // FOV
         prop = xmlGetProp (cur, TAG_FOV);
         if (prop)
            cam->fov=(atof((char*)prop) * 3.14) / 180.0;
      }
      cur = cur->next;
   }
}

/**
 * parse_objects - Parse main objects.
 * @cur: xml node
 *
 * Returns:
 * none.
 */
static void parse_objects (xmlNodePtr cur)
{
   cur = cur->xmlChildrenNode;

   while (cur)
   {
      if (!xmlStrcmp(cur->name, TAG_CAMERA))
      {
         parse_camera (cur->xmlChildrenNode);
      }
      if (!xmlStrcmp(cur->name, TAG_SPHERE))
      {
         xmlChar *prop = xmlGetProp (cur, TAG_ID);

         if (prop)
            parse_sphere (cur->xmlChildrenNode, atoi((char*)prop));
      }
      cur = cur->next;
   }
   return;
}

/**
 * xml_parse - Parse an xml file and setup the scene.
 * @docname: xml file
 *
 * This function will parse an xml file containg a scene.
 * The scene will be setup according to the information stored in the
 * xml file.
 *
 * Returns:
 * POSIX OK (zero) or non-zero on error.
 */
int xml_parse(char *docname)
{
   xmlDocPtr doc;
   xmlNodePtr cur;

   doc = xmlParseFile (docname);
   if (!doc)
   {
      fprintf (stderr, "error: Unable to parse %s.\n", docname);
      return 1;
   }

   cur = xmlDocGetRootElement (doc);
   if (!cur)
   {
      fprintf (stderr, "error: %s is empty\n", docname);
      xmlFreeDoc (doc);
      return 1;
   }
   if (xmlStrcmp (cur->name, TAG_SCENE))
   {
      fprintf (stderr, "error: %s is of the wrong type, root node != %s\n", docname, TAG_SCENE);
      xmlFreeDoc (doc);
      return 1;
   }

   parse_objects (cur);
   xmlFreeDoc (doc);

   return 0;
}

/**
 * Local Variables:
 *  c-file-style: "ellemtel"
 *  indent-tabs-mode: nil
 * End:
 */
