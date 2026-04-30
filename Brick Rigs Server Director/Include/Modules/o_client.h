#include "Module.h"
#include <string>
#include <socketio/sio_client.h>

class o_event
{
    std::string name;
    std::function<void(sio::event&)> function;
    public:

    o_event(const std::string& name, const std::function<void(sio::event&)>& function) : name(name), function(function)
    {

    }

    void Execute(sio::event& e) const
    {
        return function(e);
    }

    std::string GetName() const
    {
        return name;
    }
};

class o_client
{
    sio::client client_;
    std::vector<o_event> events_;

    o_client();
public:
    static o_client* get();

    void event_register(const o_event& e);
    void message_emit(std::string const& name, sio::message::list const& msglist = nullptr, std::function<void (sio::message::list const&)> const& ack = nullptr);
};