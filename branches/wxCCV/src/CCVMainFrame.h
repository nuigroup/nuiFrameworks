/////////////////////////////////////////////////////////////////////////////
// Name:        src/CCVMainFrame.h
// Purpose:     Provide the main frame and callback functions
// Author:      Jimbo Zhang
// Copyright:   (c) 2011 NUI Group
/////////////////////////////////////////////////////////////////////////////

#ifndef _CCV_FRAME_H
#define _CCV_FRAME_H

#include <wx/wx.h>
#include "CCVbaseFrame.h"
#include "CCVMovidProcess.h"

//
// CCVbaseMainFrame is generated automatically by wxFormBuilder, CCVMainFrame
// extends CCVbaseMainFrame to implement the event handling functions.
//
class CCVMainFrame : public CCVbaseMainFrame
{
public:
    /**
       Default constructor.
    */
    CCVMainFrame() : CCVbaseMainFrame(NULL)
    {
        movidProcess = NULL;
    }

    /**
       Set movid processor
    */
    void SetMovid(CCVMovidProcess *movidProc);

    void drawCameraImage();
    void onMovidImage(wxCommandEvent&);

private:
    CCVMovidProcess *movidProcess;
    virtual void OnInputViewerPaint(wxPaintEvent& event);
};

#endif
