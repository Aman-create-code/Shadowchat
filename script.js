/**
 * ShadowChat Ultimate V12 - REAL CALL EDITION
 */

const state = {
    user: { id: '', name: 'User' + Math.floor(Math.random() * 99), avatar: '?' },
    activeChat: null,
    chats: [],
    peer: null,
    currentCall: null,
    localStream: null,
    isRinging: false
};

const network = new BroadcastChannel('shadow_network');

// --- INITIALISIERUNG ---
document.addEventListener('DOMContentLoaded', () => {
    initUser();
    initPeer(); // Startet die Telefon-Verbindung
    setupEventListeners();
    if (window.lucide) lucide.createIcons();
});

function initUser() {
    const saved = localStorage.getItem('shadow_v12_user');
    if (saved) {
        state.user = JSON.parse(saved);
    } else {
        // WICHTIG: PeerIDs sollten keine Sonderzeichen außer Bindestriche haben
        state.user.id = 'USR-' + Math.random().toString(36).substr(2, 6).toUpperCase();
        localStorage.setItem('shadow_v12_user', JSON.stringify(state.user));
    }
    document.getElementById('myDisplayNick').innerText = state.user.name;
    document.getElementById('myFullId').innerText = state.user.id;
}

// --- ECHTE PEER-TO-PEER LOGIK ---
function initPeer() {
    // Erstellt eine Verbindung zum PeerJS-Server
    state.peer = new Peer(state.user.id);

    state.peer.on('open', (id) => {
        console.log('Meine Telefon-ID ist: ' + id);
        showToast("Online & Bereit für Anrufe", "success");
    });

    // EINGEHENDER ANRUF
    state.peer.on('call', async (incomingCall) => {
        state.currentCall = incomingCall;
        setupCallUI('incoming', incomingCall.peer);
        playSFX('ringIncoming');
        state.isRinging = true;
    });

    state.peer.on('error', (err) => {
        console.error("Peer Fehler:", err);
        showToast("Verbindungsfehler!", "error");
    });
}

// --- ANRUF STARTEN ---
window.initiateCall = async () => {
    if (!state.activeChat) return;
    const remoteId = state.activeChat.id;

    try {
        // Mikrofon anfordern
        state.localStream = await navigator.mediaDevices.getUserMedia({ audio: true, video: false });
        
        // UI anzeigen
        setupCallUI('outgoing', state.activeChat.name);
        playSFX('ringOutbound');

        // Den anderen anrufen
        const call = state.peer.call(remoteId, state.localStream);
        handleCallEvents(call);
    } catch (err) {
        showToast("Mikrofon Zugriff verweigert!", "error");
    }
};

// --- ANRUF ANNEHMEN ---
window.answerIncomingCall = async () => {
    try {
        stopAllSFX();
        state.localStream = await navigator.mediaDevices.getUserMedia({ audio: true, video: false });
        
        state.currentCall.answer(state.localStream);
        handleCallEvents(state.currentCall);

        document.getElementById('incomingCallControls').classList.add('hidden');
        document.getElementById('activeCallControls').classList.remove('hidden');
        document.getElementById('callStatus').innerText = "Verbunden";
        startCallTimer();
    } catch (err) {
        showToast("Fehler beim Annehmen", "error");
        window.hangUpCall();
    }
};

// --- STREAM HANDLING (Was passiert während des Gesprächs) ---
function handleCallEvents(call) {
    state.currentCall = call;

    call.on('stream', (remoteStream) => {
        // Den Ton des anderen auf die Lautsprecher legen
        const remoteAudio = document.getElementById('remoteAudio');
        remoteAudio.srcObject = remoteStream;
        remoteAudio.play();
        
        document.getElementById('callStatus').innerText = "Gespräch aktiv";
        document.getElementById('callRipple').classList.remove('hidden');
    });

    call.on('close', () => {
        window.hangUpCall();
    });
}

window.hangUpCall = () => {
    stopAllSFX();
    if (state.currentCall) state.currentCall.close();
    if (state.localStream) {
        state.localStream.getTracks().forEach(track => track.stop());
    }
    
    document.getElementById('callOverlay').classList.add('hidden');
    document.getElementById('callRipple').classList.add('hidden');
    clearInterval(callTimerInterval);
    state.currentCall = null;
    showToast("Anruf beendet");
};

// --- RESTLICHE UI LOGIK (Nachrichten etc.) ---
window.sendMsg = () => {
    const text = document.getElementById('msgIn').value.trim();
    if (!text) return;

    const msg = {
        id: 'MSG-' + Date.now(),
        senderId: state.user.id,
        senderName: state.user.name,
        text: text,
        timestamp: new Date().toISOString(),
        chatId: state.activeChat.id
    };

    network.postMessage({ type: 'NEW_MESSAGE', data: msg });
    appendMessage(msg, true);
    document.getElementById('msgIn').value = '';
};

network.onmessage = (event) => {
    if (event.data.type === 'NEW_MESSAGE' && state.activeChat?.id === event.data.data.chatId) {
        appendMessage(event.data.data, false);
        playSFX('message');
    }
};

// UI Hilfsfunktionen
function setupCallUI(mode, name) {
    const overlay = document.getElementById('callOverlay');
    overlay.classList.remove('hidden');
    document.getElementById('callName').innerText = name;
    if (mode === 'incoming') {
        document.getElementById('incomingCallControls').classList.remove('hidden');
        document.getElementById('activeCallControls').classList.add('hidden');
    } else {
        document.getElementById('incomingCallControls').classList.add('hidden');
        document.getElementById('activeCallControls').classList.remove('hidden');
    }
}

let callTimerInterval;
function startCallTimer() {
    const timerElem = document.getElementById('callTimer');
    timerElem.classList.remove('hidden');
    let sec = 0;
    callTimerInterval = setInterval(() => {
        sec++;
        const m = Math.floor(sec / 60).toString().padStart(2, '0');
        const s = (sec % 60).toString().padStart(2, '0');
        timerElem.innerText = `${m}:${s}`;
    }, 1000);
}

function playSFX(id) {
    const sfx = document.getElementById('sfx' + id.charAt(0).toUpperCase() + id.slice(1));
    if (sfx) sfx.play().catch(() => {});
}

function stopAllSFX() {
    ['sfxRingIncoming', 'sfxRingOutbound'].forEach(id => {
        const el = document.getElementById(id);
        el.pause(); el.currentTime = 0;
    });
}

function appendMessage(msg, isMine) {
    const container = document.getElementById('messagesList');
    const div = document.createElement('div');
    div.className = `flex ${isMine ? 'justify-end' : 'justify-start'} mb-2`;
    div.innerHTML = `<div class="p-2 rounded-lg ${isMine ? 'bg-wa-accent text-white' : 'bg-white dark:bg-wa-panel'} shadow-sm text-sm">${msg.text}</div>`;
    container.appendChild(div);
    container.scrollTo(0, container.scrollHeight);
}

// Demo-Daten initialisieren
setTimeout(() => {
    state.chats = [{ id: 'TEST-ROOM', name: 'Test Kontakt' }];
    renderChatList();
}, 500);

function renderChatList() {
    const container = document.getElementById('chatListContainer');
    container.innerHTML = state.chats.map(chat => `
        <div onclick="window.openChat('${chat.id}')" class="p-4 border-b dark:border-gray-800 cursor-pointer hover:bg-gray-100 dark:hover:bg-white/5">
            <h4 class="font-bold">${chat.name}</h4>
            <p class="text-xs text-gray-500">ID: ${chat.id}</p>
        </div>
    `).join('');
}

window.openChat = (id) => {
    state.activeChat = state.chats.find(c => c.id === id);
    document.getElementById('chatArea').classList.remove('hidden');
    document.getElementById('chatHeader').classList.remove('hidden');
    document.getElementById('chatInputArea').classList.remove('hidden');
    document.getElementById('activeTitle').innerText = state.activeChat.name;
};

window.showToast = (msg, type) => {
    console.log("Toast:", msg);
    // Hier könntest du dein Toast-Element einblenden
};
