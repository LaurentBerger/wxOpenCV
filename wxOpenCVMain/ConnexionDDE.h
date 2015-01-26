#ifndef __CONNEXIONDDE__
#define __CONNEXIONDDE__
//#include <wx/DDE.h>
#include <wx/ipc.h>
#include <wx/log.h>
// the default service name
#define IPC_SERVICE "4242"
//#define IPC_SERVICE _T("/tmp/wxsrv424")

// the hostname
#define IPC_HOST "localhost"

// the IPC topic
#define IPC_TOPIC "IPC TEST"

// the name of the item we're being advised about
#define IPC_ADVISE_NAME "Item"


// the values used by tests/benchmarks/ipcclient.cpp
#define IPC_BENCHMARK_TOPIC "wxIPC BENCH"
#define IPC_BENCHMARK_ITEM "Benchmark"

// This simple connection class adds logging of all operations
class ConnexionBase : public wxConnection
{
void	*wRef;
protected:
    void Log(const wxString& command,
             const wxString& topic,
             const wxString& item,
             const void *data,
             size_t size,
             wxIPCFormat format)
    {
        wxString s;
        if (topic.IsEmpty() && item.IsEmpty())
            s.Printf("%s(", command.c_str());
        else if (topic.IsEmpty())
            s.Printf("%s(item=\"%s\",", command.c_str(), item.c_str());
        else if (item.IsEmpty())
            s.Printf("%s(topic=\"%s\",", command.c_str(), topic.c_str());
        else
            s.Printf("%s(topic=\"%s\",item=\"%s\",", command.c_str(), topic.c_str(), item.c_str());

        switch (format)
        {
          case wxIPC_TEXT:
              s += wxString(static_cast<const char *>(data), size);
              break;

#if wxUSE_UNICODE
          case wxIPC_UNICODETEXT:
              s += wxString(static_cast<const wchar_t *>(data), size);
              break;
#endif // wxUSE_UNICODE

          case wxIPC_UTF8TEXT:
              s += wxString::FromUTF8(static_cast<const char *>(data), size);
              break;

          case wxIPC_PRIVATE:
              if ( size == 3 )
              {
                  const char *bytes = static_cast<const char *>(data);
                  s << '"' << bytes[0] << bytes[1] << bytes[2] << '"';
              }
              else
              {
                  s << "\"???\"";
              }
              break;

          case wxIPC_INVALID:
              s += "[invalid data]";
              break;

          default:
              s += "[unknown data]";
              break;
        }

        wxLogMessage("%s,%d)", s, size);
    }
    virtual bool OnExecute(const wxString& topic, const void *data, size_t size, wxIPCFormat format);
    virtual const void *OnRequest(const wxString& topic, const wxString& item, size_t *size, wxIPCFormat format);
    virtual bool OnPoke(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format);
    virtual bool OnStartAdvise(const wxString& topic, const wxString& item);
    virtual bool OnStopAdvise(const wxString& topic, const wxString& item);
    virtual bool DoAdvise(const wxString& item, const void *data, size_t size, wxIPCFormat format);
    virtual bool OnDisconnect();

    // topic for which we advise the client or empty if none

protected:
    // the data returned by last OnRequest(): we keep it in this buffer to
    // ensure that the pointer we return from OnRequest() stays valid
    wxCharBuffer m_requestData;
public :
	void DefFentrePrincipale(void *w){wRef=w;};
    wxString m_advise;
};

class ServeurScilab : public wxServer
{
void	*wRef;
public:
    ServeurScilab();
    virtual ~ServeurScilab();

    void Disconnect();
    bool IsConnected() { return m_connection != NULL; }
    ConnexionBase *GetConnection() { return m_connection; }

    void Advise();
    bool CanAdvise() { return m_connection && !m_connection->m_advise.empty();  }

    virtual ConnexionBase *OnAcceptConnection(const wxString& topic);
	void DefFentrePrincipale(void *w){wRef=w;};
protected:
    ConnexionBase *m_connection;
};

#endif
