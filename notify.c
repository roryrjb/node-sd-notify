// The MIT License (MIT)

// Copyright (c) 2017 - 2021 Rory Bradford <rory@dysfunctionalprogramming.com>
// and contributors.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define NAPI_VERSION 4
#define READY "READY=1"
#define STOPPING "STOPPING=1"
#define WATCHDOG "WATCHDOG=1"
#define MAX_LINE_SIZE 4096
#define MAX_MESSAGE_SIZE 1024

#include <node_api.h>
#include <stdio.h>
#include <systemd/sd-daemon.h>
#include <systemd/sd-journal.h>

#define debug_throw(env, m)                                                    \
  do {                                                                         \
    char err_message[MAX_MESSAGE_SIZE];                                        \
    snprintf(err_message, MAX_MESSAGE_SIZE, "(%s %s:%d) %s", __FUNCTION__,     \
             __FILE__, __LINE__, m);                                           \
    napi_throw_error(env, NULL, err_message);                                  \
  } while (0)

napi_value send_state(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_status status;
  napi_value retval;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);

  if (status != napi_ok) {
    debug_throw(env, "Error parsing arguments.");
  }

  sd_notify(0, READY);

  status = napi_get_undefined(env, &retval);

  if (status != napi_ok) {
    debug_throw(env, "napi_get_undefined");
  }

  return retval;
}

napi_value _send_state(napi_env env, const char *status) {
  napi_value retval;
  sd_notify(0, status);
  return napi_get_undefined(env, &retval);
}

napi_value stopping(napi_env env, napi_callback_info info) {
  sd_notify(0, STOPPING);
}

/* napi_value watchdog(napi_env env, napi_callback_info info) { */
/* sd_notify(0, WATCHDOG); */
/* } */

/* napi_value sendstate(napi_env env, napi_callback_info info) { */
/* napi_status status; */

/* // status = napi_get_value_string_utf8(env, args[0], */
/* //                                      char* buf, */
/* //                                      size_t bufsize, */
/* //                                      size_t* result) */
/* sd_notify(0, state); */
/* } */

// vnapi_value interval(napi_env env, napi_callback_info info) {
//   v8::Isolate *isolate = args.GetIsolate();
//   uint64_t interval;
//   int res = sd_watchdog_enabled(0, &interval);

//   if (res > 0) {
//     args.GetReturnValue().Set(v8::Number::New(isolate, interval / 1000));
//   } else {
//     args.GetReturnValue().Set(v8::Number::New(isolate, 0));
//   }
// }

// napi_value journal_print(napi_env env, napi_callback_info info) {
//   int level = args[0].As<Number>()->Value();
//   v8::Isolate *isolate = args.GetIsolate();
//   v8::String::Utf8Value str(isolate, args[1]);
//   const char *message = ToCString(str);
//   sd_journal_print(level, message);
// }

napi_value entrypoint(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, ready, NULL, &fn);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function.");
  }

  if ((napi_set_named_property(env, exports, "ready", fn)) != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports.");
  }

  /* if ((napi_set_named_property(env, exports, "MAX_LINE_SIZE", MAX_LINE_SIZE))
   * != */
  /* napi_ok) { */
  /* napi_throw_error(env, NULL, "Unable to populate exports."); */
  /* } */

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, entrypoint)
