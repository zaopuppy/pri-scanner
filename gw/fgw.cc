#include <iostream>
#include <signal.h>

#include <event2/event.h>

#include "libframework/zframework.h"

#include "zserial.h"
#include "fgw_client.h"
#include "upload_worker.h"

using namespace std;

static volatile bool g_stop = false;

// void (*signal(int sig, void (*func)(int)))(int);
void signal_handler(int sig)
{
  switch (sig) {
    case SIGTERM:
      printf("SIGTERM received, exit\n");
      g_stop = true;
      break;
    default:
      // ignore
      break;
  }
}

int main(int argc, char *argv[])
{
  sig_t sig_ret = signal(SIGTERM, signal_handler);
  if (sig_ret == SIG_ERR) {
    printf("failed to set signal handler\n");
    return -1;
  }

  z_log_init("fgw.conf", "fgw");

  struct event_base* base = event_base_new();
  assert(base);

  int rv = FAIL;
  do {
    ZDispatcher::init(base);

    ZModule *serial = NULL;
    {
      // const char *serial_dev = "/dev/ttyUSB0";
      // const char *serial_dev = "/dev/tty.usbserial-FTG5WHHL";
      const char *serial_dev = "/dev/tty.usbserial-ftDX0P76";
      serial = new ZSerial(base, serial_dev);
      if (serial->init() != OK) {
        Z_LOG_D("failed to init serial module.");
        break;
      }
    }

    FGWClient *client = NULL;
    {
      client = new FGWClient(base);
      client->setServerAddress("192.168.2.105", 1984);
      if (OK != client->init()) {
        Z_LOG_E("failed to initialize fgw client");
        break;
      }
    }

    //UploadWorker worker;
    //if (!worker.init()) {
    //  Z_LOG_E("failed to initialize upload worker");
    //  break;
    //}
    //
    //if (!worker.start()) {
    //  Z_LOG_E("failed to start upload worker");
    //  break;
    //}

    while (!g_stop) {
      event_base_loop(base, EVLOOP_ONCE);
      serial->checkMsgQueue();
      client->checkMsgQueue();
    }

    // worker.stop();

    rv = OK;
  } while (0);

  z_log_fini();

  return rv;
}


