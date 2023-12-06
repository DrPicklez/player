#ifndef GSTREAMERPLAYER_H
#define GSTREAMERPLAYER_H

#include <gst/gst.h>
#include <glib.h>
#include "statemachine.h"
#include <thread>
#include <mutex>
/*
class GstreamerPlayer
{
public:
    static gboolean on_message(GstBus *bus, GstMessage *message, gpointer user_data);
    static gboolean query_position(GstElement *pipeline);
    static void seek_to_time (GstElement *pipeline, gint64 time_nanoseconds);
    void StartPipeline(int argc, char* arvgPtr);
    GstreamerPlayer();
};
*/
#endif // GSTREAMERPLAYER_H
