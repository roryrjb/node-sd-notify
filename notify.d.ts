declare enum logLevel {
  EMERG,
  ALERT,
  CRIT,
  ERR,
  WARNING,
  NOTICE,
  INFO,
  DEBUG
}

export const PID_MAX_LIMIT: 4194304

export function journalPrint(level: logLevel, text: string): void

export function ready(pid: number): void

export function watchdog(interval: number): void

export function sendState(text: string): void