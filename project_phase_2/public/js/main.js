// video 標籤
const localVideo = document.querySelector('video#localVideo')
const remoteVideo = document.querySelector('video#remoteVideo')

// button 標籤
const startBtn = document.querySelector('button#startBtn')
const leaveBtn = document.querySelector('button#leaveBtn')

let localStream
let peerConn
let socket
const room = 'room1'

function connectIO() {
  // socket
  const ws = new WebSocket("wss://" + location.host + "/")
  //socket = io('wss://location.host:1025')
  socket = io(ws)
  console.log('connect IO')

  socket.on('ready', async (msg) => {
    console.log(msg)
    // 發送 offer
    console.log('發送 offer ')
    await sendSDP(true)
  })

  socket.on('ice_candidate', async (data) => {
    console.log('收到 ice_candidate')
    const candidate = new RTCIceCandidate({
      sdpMLineIndex: data.label,
      candidate: data.candidate,
    })
    await peerConn.addIceCandidate(candidate)
  })

  socket.on('offer', async (desc) => {
    console.log('收到 offer')
    // 設定對方的配置
    await peerConn.setRemoteDescription(desc)

    // 發送 answer
    await sendSDP(false)
  })

  socket.on('answer', async (desc) => {
    console.log('收到 answer')

    // 設定對方的配置
    await peerConn.setRemoteDescription(desc)
  })

  socket.on('leaved', () => {
    console.log('收到 leaved')
    socket.disconnect()
    closeLocalMedia()
  })

  socket.on('bye', () => {
    console.log('收到 bye')
    hangup()
  })

  socket.emit('join', room)
}

/**
 * 取得本地串流
 */
async function createStream() {
  try {
    // 取得影音的Stream
    const stream = await navigator.mediaDevices.getUserMedia({
      audio: true,
      video: true,
    })

    // 提升作用域
    localStream = stream

    // 導入<video>
    localVideo.srcObject = stream
  } catch (err) {
    throw err
  }
}

/**
 * 初始化Peer連結
 */
function initPeerConnection() {
  const configuration = {
    iceServers: [
      {
        urls: 'stun:stun.l.google.com:19302',
      },
    ],
  }
  peerConn = new RTCPeerConnection(configuration)

  // 增加本地串流
  localStream.getTracks().forEach((track) => {
    peerConn.addTrack(track, localStream)
  })

  // 找尋到 ICE 候選位置後，送去 Server 與另一位配對
  peerConn.onicecandidate = (e) => {
    if (e.candidate) {
      console.log('發送 ICE')
      // 發送 ICE
      socket.emit('ice_candidate', room, {
        label: e.candidate.sdpMLineIndex,
        id: e.candidate.sdpMid,
        candidate: e.candidate.candidate,
      })
    }
  }

  // 監聽 ICE 連接狀態
  peerConn.oniceconnectionstatechange = (e) => {
    if (e.target.iceConnectionState === 'disconnected') {
      remoteVideo.srcObject = null
    }
  }

  // 監聽是否有流傳入，如果有的話就顯示影像
  peerConn.onaddstream = ({ stream }) => {
    // 接收流並顯示遠端視訊
    remoteVideo.srcObject = stream
  }
}

/**
 * 處理信令
 * @param {Boolean} isOffer 是 offer 還是 answer
 */
async function sendSDP(isOffer) {
  try {
    if (!peerConn) {
      initPeerConnection()
    }

    // 創建SDP信令
    const localSDP = await peerConn[isOffer ? 'createOffer' : 'createAnswer']({
      offerToReceiveAudio: true, // 是否傳送聲音流給對方
      offerToReceiveVideo: true, // 是否傳送影像流給對方
    })

    // 設定本地SDP信令
    await peerConn.setLocalDescription(localSDP)

    // 寄出SDP信令
    let e = isOffer ? 'offer' : 'answer'
    socket.emit(e, room, peerConn.localDescription)
  } catch (err) {
    throw err
  }
}

/**
 * 關閉自己的視訊
 */
function closeLocalMedia() {
  if (localStream && localStream.getTracks()) {
    localStream.getTracks().forEach((track) => {
      track.stop()
    })
  }
  localStream = null
}

/**
 * 掛掉電話
 */
function hangup() {
  if (peerConn) {
    peerConn.close()
    peerConn = null
  }
}

/**
 * 初始化
 */
async function init() {
  await createStream()
  initPeerConnection()
  connectIO()
  startBtn.disabled = true
  leaveBtn.disabled = false
}

// window.onload = init()

leaveBtn.onclick = () => {
  if (socket) {
    socket.emit('leave', room)
  }
  hangup()
  startBtn.disabled = false
  leaveBtn.disabled = true
}

startBtn.onclick = init
