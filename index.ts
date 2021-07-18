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

import * as assert from 'assert'
import * as notify from 'notify'

let watchdogTimer: NodeJS.Timeout

function journalPrint (...args: any[]): void {
  const level = args.shift()
  notify.journalPrint(level, args.join(' '))
}

const log = {
  emerg: journalPrint.bind(null, notify.logLevel.EMERG),
  emergency: journalPrint.bind(null, notify.logLevel.EMERG),
  alert: journalPrint.bind(null, notify.logLevel.ALERT),
  crit: journalPrint.bind(null, notify.logLevel.CRIT),
  critical: journalPrint.bind(null, notify.logLevel.CRIT),
  err: journalPrint.bind(null, notify.logLevel.ERR),
  error: journalPrint.bind(null, notify.logLevel.ERR),
  warning: journalPrint.bind(null, notify.logLevel.WARNING),
  warn: journalPrint.bind(null, notify.logLevel.WARNING),
  notice: journalPrint.bind(null, notify.logLevel.NOTICE),
  info: journalPrint.bind(null, notify.logLevel.INFO),
  debug: journalPrint.bind(null, notify.logLevel.DEBUG)
}

module.exports = Object.assign({}, notify, {
  ready(pid?: number): void {
    if (pid !== undefined) {
      assert(pid > 0 && pid <= notify.PID_MAX_LIMIT, 'pid out of range.')
    }

    notify.ready(pid || process.pid)
  },

  startWatchdogMode(interval: number): void {
    interval = interval || 3000
    watchdogTimer = setInterval(notify.watchdog, interval)
  },

  stopWatchdogMode(): void {
    if (watchdogTimer) {
      clearInterval(watchdogTimer)
      watchdogTimer = null
    }
  },

  sendStatus(text: string): void {
    notify.sendState('STATUS=' + text + '\n')
  },

  log: log
})