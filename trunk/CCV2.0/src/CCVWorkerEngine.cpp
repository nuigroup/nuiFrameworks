/////////////////////////////////////////////////////////////////////////////
// Name:        src/CCVWorkerEngine.cpp
// Purpose:     Provide the thread for movid processing
// Author:      Jimbo Zhang
// Copyright:   (c) 2011 NUI Group
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/log.h>
#include "CCVWorkerEngine.h"
#include "otMovidStreamModule.h"

int g_config_delay = 50;

const wxEventType newEVT_MOVIDPROCESS_NEWIMAGE = wxNewEventType();

CCVWorkerEngine::CCVWorkerEngine()
{
    eventHandler = NULL;
    procGraph = new CCVProcGraph();
}

void *CCVWorkerEngine::Entry()
{
    while (true) {
        wxThread::Sleep(20);
        
        if (!procGraph || ! procGraph->isStarted())
            continue;
            
        if (! procGraph->hasLocked() && procGraph->isStarted())
            procGraph->poll();

        while (procGraph->haveError())
            wxLogMessage(wxT("procGraph error: %s"), procGraph->getLastError().c_str());
            
        ModuleList outputModules = procGraph->GetOutputModules();
        
        for (ModuleList::const_iterator iter = outputModules.begin();
	     iter != outputModules.end(); ++iter) {
	        moModule *theModule = iter->first;
	        std::string moduleLable = iter->second;
	        
	        RGBRawImage outRGBRaw;
	        CvSize *outRoi = new CvSize;
	        
            if (! procGraph->hasLocked() && procGraph->size()>0 && theModule->getName() == "Stream") {
                otStreamModule *stream = (otStreamModule *)theModule;
                if (stream->copy()) {
                    cvGetRawData(stream->output_buffer, &outRGBRaw, NULL, outRoi);                    
                }
            }
            
            outImages.push_back(new OutRGBImage(outRGBRaw, outRoi, moduleLable));
        }
        
        if (eventHandler != NULL) {
            wxCommandEvent event( newEVT_MOVIDPROCESS_NEWIMAGE, GetId() );
            if(!TestDestroy()) {
                wxPostEvent(eventHandler, event);
            }
        }
    }

    return NULL;
}
