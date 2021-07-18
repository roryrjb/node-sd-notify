import * as notify from '../../'
import * as express from 'express'

const PORT = 8000

const app = express()

app.listen(PORT, async () => {
  console.log(`listening on port ${PORT}`)

  // simulate some startup delay
  await new Promise((resolve) => setTimeout(resolve, 1000))

  notify.ready()
  notify.startWatchdogMode(2800)
  notify.sendStatus('send some status to systemd')
  notify.log.warn('some warning')
})