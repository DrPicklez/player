#include "gstreamerplayer.h"
/*
// gst-launch-1.0 filesrc location=bear_mjpeg.mov ! qtdemux ! videorate ! jpegdec ! autovideosink
GstreamerPlayer::GstreamerPlayer()
{

}

static gboolean on_message(GstBus *bus, GstMessage *message, gpointer user_data) {
    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit((GMainLoop*)user_data);
            break;
        case GST_MESSAGE_ERROR: {
            GError *error;
            gchar *debug_info;
            gst_message_parse_error(message, &error, &debug_info);
            g_printerr("Error: %s\n", error->message);
            g_error_free(error);
            g_free(debug_info);
            g_main_loop_quit((GMainLoop*)user_data);
            break;
        }
        default:
            break;
    }
    return TRUE;
}




static gboolean query_position(GstElement *pipeline){
    gint64 pos, len;
    if(gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)){

//        g_print("Time: %" GST_TIME_FORMAT "\r", GST_TIME_ARGS (pos));
    }
//    std::cout << std::string("position!") << std::endl;
    return TRUE;
}

static void seek_to_time (GstElement *pipeline, gint64 time_nanoseconds){
  if (!gst_element_seek (pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time_nanoseconds,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
    g_print ("Seek failed!\n");
  }
}

void GstreamerPlayer::StartPipeline(int argc, char arvgPtr){
    StateMachine stateMachine;
    stateMachine.parseFile();
    gst_init(&argc, &arvgPtr);


    GstElement *pipeline = gst_pipeline_new("pipeline");

    GstElement *src = gst_element_factory_make("filesrc", "source");
    GstElement *demuxer = gst_element_factory_make("qtdemux", "demuxer");
    GstElement *decoder = gst_element_factory_make("jpegdec", "decoder");
    GstElement *convert = gst_element_factory_make("videoconvert", "convert");
    GstElement *videorate = gst_element_factory_make("videorate", "rate");
    GstElement *sink = gst_element_factory_make("autovideosink", "sink");

    gst_bin_add_many(GST_BIN(pipeline), src, demuxer, videorate, decoder, convert, sink, NULL);

    g_object_set(G_OBJECT(src), "location", "/home/william/Projects/videoPlayer/bear_mjpeg.mov", NULL);

    gst_element_link_many(src, decoder, convert, sink, NULL);

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    g_signal_connect(bus, "message", G_CALLBACK(on_message), loop);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
//    gst_element_seek_simple(pipeline, GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH, 30 * GST_SECOND);
    g_timeout_add(200, (GSourceFunc) query_position, pipeline);
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}
*/
