/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Clamp pixel values to a threshold

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_pix_background_depth_H_
#define _INCLUDE__GEM_PIXES_pix_background_depth_H_

#include "Base/GemPixObj.h"
#include <RTE/MessageCallbacks.h>


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_background_depth
    
    Clamp pixel values to a threshold

KEYWORDS
    pix
    
DESCRIPTION

    Subtract a ref depth map to a depth stream from a Kinect sensor
   
-----------------------------------------------------------------*/
#ifdef _WIN32
class GEM_EXPORT pix_background_depth : public GemPixObj
#else
class GEM_EXTERN pix_background_depth : public GemPixObj
#endif
{
    CPPEXTERN_HEADER(pix_background_depth, GemPixObj);

    public:

        //////////
        // Constructor
    	pix_background_depth(t_floatarg f);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_background_depth();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	

    	//////////

    	//////////
    	// Set the new threshold value
    static void    	setrefMess(void *data);
    static void	    threshMess(void *data, t_floatarg value);
    static void	    modeMess(void *data, t_floatarg value);
    static void     activeMessCallback(void *data, t_floatarg value);
  
    bool m_active; // switch on/off processing
  
		int m_thresh;
		bool m_setref;
        bool m_mode;
		std::vector<int> buffer;
};

#endif	// for header file
