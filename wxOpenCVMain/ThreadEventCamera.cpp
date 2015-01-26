#include "EvenementCamera.h"



ProcessGestionCamera::ProcessGestionCamera(ControleCamera *c,CameraVirtuelle *cc)
{
ctrlCam=c;
cam=cc;
}

void *ProcessGestionCamera::Entry()
{
#ifdef __WINDOWS__
unsigned long taille;
unsigned long maxTaille=1004*1002; 
HANDLE hEvent = OpenEvent ( EVENT_ALL_ACCESS , false, _T("ANDOREVENT" ));
if ( !hEvent ) { return NULL; }
// Loop through and wait for an event to occur
if (cam->TestDriver())
	{
	while(TRUE)
		{
	// Wait for the Event

		WaitForSingleObject ( hEvent, INFINITE );
		if (cam->ImagePrete())
	//    No need to Reset the event as its become non signaled as soon as
			if (ctrlCam->CommunicationEvenement())
				{
				wxCommandEvent c(wxEVT_COMMAND_MENU_SELECTED, 358);
				c.SetInt( m_count );
				wxPostEvent(ctrlCam,c);
				cam->TailleImage(taille);
				wxThread::Sleep(400*taille/maxTaille);
				}
			else
				{
				cam->TailleImage(taille);
				ctrlCam->DefImagePrete(1);
				wxThread::Sleep(300*taille/maxTaille);
				}
	//    some thread catches the event.
		SetEvent(NULL);
		ResetEvent(hEvent);

		}
	CloseHandle(hEvent);
	return NULL;
	}
else
	while(TRUE)
		{
		if (cam->ImagePrete())
			{
			wxCommandEvent c(wxEVT_COMMAND_MENU_SELECTED, 358);
			c.SetInt( m_count );
			wxPostEvent(ctrlCam,c);
			cam->TailleImage(taille);
			wxThread::Sleep(500*taille/maxTaille);
			//wxThread::Sleep(370*taille/maxTaille);
			}
		}
#endif
return NULL;
}


