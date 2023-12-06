#include <iostream>
#include <gst/gst.h>
#include <glib.h>
#include "statemachine.h"
#include "gstreamerplayer.h"
#include <thread>
#include <mutex>


// gst-launch-1.0 filesrc location=/home/william/Projects/videoPlayer/bear_mjpeg.mov ! qtdemux ! videorate ! jpegdec ! autovideosink
//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

StateMachine stateMachine;
gint fps = 60;
//int ms = int(float(1./fps) * 1000);

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

//This is for linking qtdemuxer to jpeg decoder, once it needs a frame decoding.
static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;
    /* We can now link this pad with the vorbis-decoder sink pad */
    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);
}


static void seek_to_time (GstElement *pipeline, gint64 time_nanoseconds){
  if (!gst_element_seek (pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time_nanoseconds,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
        g_print ("Seek failed!\n");
  }
}

static void seek_to_frame(GstElement *pipeline, int frame_number, int frame_rate) {
  gint64 position_ns = frame_number * GST_SECOND / frame_rate;
  GstEvent *seek_event = gst_event_new_seek(1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                                            GST_SEEK_TYPE_SET, position_ns,
                                            GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);

  gst_element_send_event(pipeline, seek_event);
  std::cout << "seeking to time_ns: " << position_ns  << std::endl;
}

bool init = true;
static gboolean query_position(GstElement *pipeline){
    gint64 pos;

    if(gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)){

        gint64 frame = pos / (GST_SECOND / fps);
//        std::cout << frame << std::endl;

        // First Run
        if(init){
            seek_to_frame(pipeline, stateMachine.currentState.startTime, fps);
            init = false;
            return TRUE;
        }

        if(frame >=stateMachine.currentState.endTime){
            std::cout << "end of clip: " << stateMachine.currentState.name << ": " << frame << std::endl;
            std::cout << std::endl;
            stateMachine.currentState = stateMachine.getNextState();
            std::cout << "next state: " << stateMachine.currentState.name << std::endl;
            seek_to_frame(pipeline, stateMachine.currentState.startTime, fps);
            std::cout << "seeking to frame: " << stateMachine.currentState.startTime << std::endl;
        }
    }
    return TRUE;

}


//#define GST_VERBOSE 1
int main(int argc, char *argv[]) {
    stateMachine.parseFile();
    gst_init(&argc, &argv);

    GstElement *pipeline = gst_pipeline_new("pipeline");
    GstElement *filesrc = gst_element_factory_make("filesrc", "filesrc");
    GstElement *qtdemux = gst_element_factory_make("qtdemux", "qtdemux");
    GstElement *queue = gst_element_factory_make("queue", "queue");
    GstElement *videorate = gst_element_factory_make("videorate", "videorate");
    GstElement *jpegdec = gst_element_factory_make("jpegdec", "jpegdec");
    GstElement *avdec_h264 = gst_element_factory_make("avdec_h264", "avdec_h264");
    GstElement *uvch264mjpgdemux = gst_element_factory_make("uvch264mjpgdemux", "uvch264mjpgdemux");
    GstElement *decodebin = gst_element_factory_make("decodebin", "decodebin");
    GstElement *h264parse = gst_element_factory_make("h264parse", "h264parse");
    GstElement *videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    GstElement *videoscale = gst_element_factory_make("videoscale", "videoscale");
    GstElement *autovideosink = gst_element_factory_make("autovideosink", "autovideosink");
    GstElement *xvimagesink = gst_element_factory_make("xvimagesink", "xvimagesink");

    GstCaps *caps = gst_caps_new_simple (
       "image/jpeg",
       "framerate", GST_TYPE_FRACTION, 30, 1,
       "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
       "width", G_TYPE_INT, 1920,
       "height", G_TYPE_INT, 1080,
       NULL);


//gst-launch-1.0 -v filesrc location=/home/william/Projects/videoPlayer/bear_mjpeg.mov ! qtdemux ! image/jpeg, parsed=true, width=1920, height=1080, framerate=30/1, pixel-aspect-ratio=1/1 ! jpegdec  ! xvimagesink
    g_object_set(G_OBJECT(filesrc), "location", "/home/william/Projects/videoPlayer/bear_mjpeg.mov", NULL);
    gst_bin_add_many(GST_BIN(pipeline), filesrc, qtdemux, jpegdec, videoconvert, autovideosink, NULL);

    /* we link the elements together */
    /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
    gst_element_link (filesrc, qtdemux);
    gst_element_link_many (jpegdec, videoconvert, autovideosink, NULL);
    g_signal_connect (qtdemux, "pad-added", G_CALLBACK (on_pad_added), jpegdec);


    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_signal_connect(bus, "message", G_CALLBACK(on_message), loop);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_timeout_add(1.0, (GSourceFunc) query_position, pipeline);     //1.0 is a Gst clock
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}


/*
 * Alternative Pipeline h264
//gst-launch-1.0 -v filesrc location=/home/william/Projects/videoPlayer/Player/Bear/Video.mp4 ! qtdemux ! h264parse ! avdec_h264 ! autovideosink
    g_object_set(G_OBJECT(filesrc), "location", "/home/william/Projects/videoPlayer/Player/Bear/Video.mp4", NULL);
    gst_bin_add_many(GST_BIN(pipeline), filesrc, qtdemux, h264parse, avdec_h264, videoconvert, autovideosink, NULL);
    if(!gst_element_link_many(filesrc, qtdemux, h264parse, avdec_h264, videoconvert, autovideosink, NULL)){
        std::cout << "link error" << std::endl;
    }
*/
