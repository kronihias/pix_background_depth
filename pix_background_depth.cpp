////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


#include "pix_background_depth.h"
#include <stdio.h>
#include <math.h>

CPPEXTERN_NEW_WITH_ONE_ARG(pix_background_depth,t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// pix_background_depth
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_background_depth :: pix_background_depth(t_floatarg f) : m_setref(true), m_mode(false), m_active(true)
{
    if (f == 0) {
        m_thresh = 100;
    } else {
        m_thresh = (int) f;
    }
    // inlet threshold
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("thresh"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_background_depth :: ~pix_background_depth()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_background_depth :: processRGBAImage(imageStruct &image)
{
  if (m_active) { // compute just if active
    int datasize = image.xsize * image.ysize;
    int i;
    
    if ( m_setref ){
      buffer.clear();
    }
    
    unsigned char *base = image.data;
    
    int value = 0;
    int userid = 0;
    
    for(i=0; i<datasize ; i++)    {
      if (base[chAlpha]) { // filter out alpha=0 values
        value = ((int)base[chRed] << 8) + (int)base[chGreen];
        
        if ( m_setref ){
          buffer.push_back(value); // set value for subtraction
        }
        
        if (!m_mode) { // different threshold computations
          if ( value > (buffer[i] - m_thresh) ) {
            base[chAlpha] = 0; // just set alpha zero
          }
        } else {
          if ( abs(value - buffer[i]) < abs(m_thresh) ) {
            base[chAlpha] = 0; // just set alpha zero
          }
        }
      }
      base += 4;
    }
    
    if ( m_setref ){
      m_setref = false;
    }
  }
}


void pix_background_depth :: setrefMess(void *data)
{
    // copy current depth map to buf
    GetMyClass(data)->m_setref = true;
}

void pix_background_depth :: threshMess(void *data, t_floatarg value)
{
    GetMyClass(data)->m_thresh = (int) value;
}

void pix_background_depth :: modeMess(void *data, t_floatarg value)
{
    if ((float)value < 0.5) {
        GetMyClass(data)->m_mode=false;
    } else {
        GetMyClass(data)->m_mode=true;
    }
}

void pix_background_depth :: activeMessCallback(void *data, t_floatarg value)
{
  if (value < 0.5) {
    GetMyClass(data)->m_active = false;
  } else {
    GetMyClass(data)->m_active = true;
  }
  
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_background_depth :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, reinterpret_cast<t_method>(&pix_background_depth::activeMessCallback));
    class_addbang(classPtr, reinterpret_cast<t_method>(&pix_background_depth::setrefMess));
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_background_depth::setrefMess),
                    gensym("setref"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_background_depth::threshMess),
                    gensym("thresh"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_background_depth::modeMess),
                    gensym("mode"), A_FLOAT, A_NULL);
}
