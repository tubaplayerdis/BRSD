#pragma once

#include <string>
#include <socketio/sio_client.h>

/**
 * @brief Defines a network event.
 */
class o_event
{
    std::string name_;
    std::function<void(sio::event&)> function_;
    public:

    /**
     * @brief Create a network event with a specific name that can be called. Events are automatically added to the client singleton
     * @param name Name of the event
     * @param function Function that will be called when the event takes place
     */
    o_event(std::string name, const std::function<void(sio::event&)>& function);

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
    friend class std::default_delete<o_client>;

    sio::client client_;
    std::vector<o_event*> events_;

    void event_register(o_event* e);

    o_client();
    ~o_client();
public:

    static o_client* get();

    void message_emit(std::string const& name, sio::message::list const& msglist = nullptr, std::function<void (sio::message::list const&)> const& ack = nullptr);
};