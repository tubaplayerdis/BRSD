#pragma once

#include <string>
#include <socketio/sio_client.h>

class o_event
{
    std::string name_;
    std::function<void(sio::event&)> function_;
    public:

    o_event(const std::string& name, const std::function<void(sio::event&)>& function);

    void execute(sio::event& e) const
    {
        return function_(e);
    }

    [[nodiscard]] std::string get_name() const
    {
        return name_;
    }
};

class o_client
{
    friend class o_event;

    sio::client client_;
    std::vector<o_event> events_;

    void event_register(const o_event& e);
    //Called internally by oevent

    o_client();
public:
    ~o_client();

    static o_client* get();

    void message_emit(std::string const& name, sio::message::list const& msglist = nullptr, std::function<void (sio::message::list const&)> const& ack = nullptr);
};