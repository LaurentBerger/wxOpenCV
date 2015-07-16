#include "FenetrePrincipale.h"
#include "wx/wxprec.h"
#include "ConnexionDDE.h"

ServeurScilab::ServeurScilab() : wxServer()
{
    m_connection = NULL;
}

ServeurScilab::~ServeurScilab()
{
    Disconnect();
}

ConnexionBase *ServeurScilab::OnAcceptConnection(const wxString& topic)
{
    wxLogWarning("OnAcceptConnection(\"%s\")", topic);

    if ( topic == IPC_TOPIC )
    {
        m_connection = new ConnexionBase();
        m_connection->DefFentrePrincipale(wRef);
        wxLogWarning("Connection accepted");
        return m_connection;
    }
    //else: unknown topic

    wxLogWarning("Unknown topic, connection refused");
    return NULL;
}

void ServeurScilab::Disconnect()
{
    if ( m_connection )
    {
        delete m_connection;
        m_connection = NULL;
        wxLogWarning("Disconnected client");
    }
}

void ServeurScilab::Advise()
{
    if ( CanAdvise() )
    {
        const wxDateTime now = wxDateTime::Now();

        m_connection->Advise(m_connection->m_advise, now.Format());

        const wxString s = now.FormatTime() + " " + now.FormatDate();
        m_connection->Advise(m_connection->m_advise, s.mb_str(), wxNO_LEN);

#if wxUSE_DDE_FOR_IPC
        wxLogWarning("DDE Advise type argument cannot be wxIPC_PRIVATE. "
                     "The client will receive it as wxIPC_TEXT, "
                     " and receive the correct no of bytes, "
                     "but not print a correct log entry.");
#endif
        char bytes[3] = { '1', '2', '3' };
        m_connection->Advise(m_connection->m_advise, bytes, 3, wxIPC_PRIVATE);
    }
}

// ----------------------------------------------------------------------------
// MyConnection
// ----------------------------------------------------------------------------

bool
ConnexionBase::OnExecute(const wxString& topic,
                        const void *data,
                        size_t size,
                        wxIPCFormat format)
{
    Log("OnExecute", topic, "", data, size, format);
    return true;
}

bool
ConnexionBase::OnPoke(const wxString& topic,
                     const wxString& item,
                     const void *data,
                     size_t size,
                     wxIPCFormat format)
{
    Log("OnPoke", topic, item, data, size, format);
    return wxConnection::OnPoke(topic, item, data, size, format);
}

const void *
ConnexionBase::OnRequest(const wxString& topic,
                        const wxString& item,
                        size_t *size,
                        wxIPCFormat format)
{
    *size = 0;
    static unsigned char*  data;
	static char	tmp[20];
    wxString afterDate;
    if ( item.StartsWith("CLASSE IMAGE", &afterDate) )
		{
		cv::Mat matIm=((FenetrePrincipale*)wRef)->ImAcq()->getMat(cv::ACCESS_READ);
		sprintf(tmp,"%d %d",matIm.rows,matIm.cols);
        *size = sizeof(ImageInfoCV);
		data = (unsigned char*)&matIm;
		}
    if ( item.StartsWith("Data", &afterDate) )
		{
		int pos=item.find("Data");
		wxString numLigne=item.substr(pos+4,item.length()-4);
		long indLigne;
		numLigne.ToLong(&indLigne);
		int nbLigne = 0xFFFF & indLigne;
		indLigne = (0xFFFF0000 & indLigne)>>16;

		cv::Mat matIm=((FenetrePrincipale*)wRef)->ImAcq()->getMat(cv::ACCESS_READ);
        *size = matIm.rows*matIm.cols;
        data =matIm.ptr(0);

		}

    if ( !*size )
    {
        wxLogWarning("Unknown request for \"%s\"", item);
        return NULL;
    }

    // store the data pointer to which we return in a member variable to ensure
    // that the pointer remains valid even after we return
   // Log("OnRequest", topic, item, data, *size, format);
    return data;
}

bool ConnexionBase::OnStartAdvise(const wxString& topic, const wxString& item)
{
    wxLogWarning("OnStartAdvise(\"%s\", \"%s\")", topic, item);
    wxLogWarning("Returning true");
    m_advise = item;
    return true;
}

bool ConnexionBase::OnStopAdvise(const wxString& topic, const wxString& item)
{
    wxLogWarning("OnStopAdvise(\"%s\",\"%s\")", topic, item);
    wxLogWarning("Returning true");
    m_advise.clear();
    return true;
}

bool
ConnexionBase::DoAdvise(const wxString& item,
                       const void *data,
                       size_t size,
                       wxIPCFormat format)
{
    Log("Advise", "", item, data, size, format);
    return wxConnection::DoAdvise(item, data, size, format);
}

bool ConnexionBase::OnDisconnect()
{
    wxLogWarning("OnDisconnect()");
    return true;
}
