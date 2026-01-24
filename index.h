<!DOCTYPE html>
<html lang="de" class="dark">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover">
    <title>ShadowChat Ultimate V8 - PRO</title>
    
    <script src="https://cdn.tailwindcss.com"></script>
    <script src="https://unpkg.com/lucide@latest"></script>
    
    <script>
        tailwind.config = {
            darkMode: 'class',
            theme: {
                extend: {
                    colors: {
                        wa: { 
                            light: '#f0f2f5', 
                            dark: '#0b141a', 
                            panel: '#202c33',
                            accent: '#00a884', 
                            bubbleOut: '#d9fdd3', 
                            bubbleIn: '#ffffff', 
                            bubbleDarkOut: '#005c4b', 
                            bubbleDarkIn: '#233138',
                            iosRed: '#ff3b30',
                            iosGreen: '#34c759'
                        }
                    }
                }
            }
        }
    </script>

    <style>
        :root { --sat: env(safe-area-inset-top); --sab: env(safe-area-inset-bottom); }
        body { font-family: 'SF Pro Display', sans-serif; overflow: hidden; height: 100dvh; background-color: #0b141a; }
        .custom-scrollbar::-webkit-scrollbar { width: 4px; }
        .custom-scrollbar::-webkit-scrollbar-thumb { background: #374151; border-radius: 10px; }
        .glass-modal { backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); background-color: rgba(0,0,0,0.6); }
        .tab-transition { transition: all 0.3s ease; }
        .msg-anim { animation: msgSlide 0.15s ease-out forwards; }
        @keyframes msgSlide { from { opacity: 0; transform: scale(0.95); } to { opacity: 1; transform: scale(1); } }
    </style>
</head>
<body class="text-gray-900 dark:text-gray-100 flex flex-col md:flex-row">

    <audio id="remoteAudio" autoplay playsinline></audio>
    <audio id="localAudio" muted autoplay playsinline></audio>

    <aside id="mainSidebar" class="flex flex-col w-full md:w-[350px] lg:w-[420px] h-full border-r border-gray-200 dark:border-gray-800 bg-white dark:bg-wa-dark z-20 shrink-0 overflow-hidden transition-all">
        <header class="h-[64px] bg-wa-light dark:bg-wa-panel flex items-center justify-between px-4 shrink-0">
            <div class="flex items-center gap-3">
                <div id="myAvatar" class="w-10 h-10 rounded-full bg-wa-accent flex items-center justify-center font-bold text-white cursor-pointer" onclick="window.switchTab('settings')">?</div>
                <div>
                    <p class="font-bold text-sm leading-none" id="myDisplayNick">Lädt...</p>
                    <p class="text-[10px] text-wa-accent font-bold mt-1 uppercase tracking-tighter">ShadowChat V8 PRO</p>
                </div>
            </div>
            <div class="flex items-center gap-1">
                <button onclick="window.switchTab('status')" class="p-2 rounded-full hover:bg-gray-200 dark:hover:bg-gray-700"><i data-lucide="circle-dashed" class="w-5 h-5"></i></button>
                <button onclick="window.toggleModal('newGroupModal')" class="p-2 rounded-full hover:bg-gray-200 dark:hover:bg-gray-700"><i data-lucide="plus" class="w-5 h-5"></i></button>
                <button onclick="window.toggleModal('joinGroupModal')" class="p-2 rounded-full hover:bg-gray-200 dark:hover:bg-gray-700"><i data-lucide="hash" class="w-5 h-5"></i></button>
                <button onclick="window.toggleTheme()" class="p-2 rounded-full hover:bg-gray-200 dark:hover:bg-gray-700"><i data-lucide="sun-moon" class="w-5 h-5"></i></button>
            </div>
        </header>

        <div class="p-2 bg-white dark:bg-wa-dark border-b dark:border-gray-800">
            <div class="bg-gray-100 dark:bg-wa-panel flex items-center px-3 py-1.5 rounded-lg">
                <i data-lucide="search" class="w-4 h-4 text-gray-400"></i>
                <input type="text" placeholder="Suchen..." class="bg-transparent text-sm w-full outline-none px-3 py-1">
            </div>
        </div>

        <div class="flex-1 overflow-y-auto custom-scrollbar relative">
            <div id="tab-chats" class="tab-transition h-full">
                <div id="chatListContainer" class="divide-y dark:divide-gray-800"></div>
            </div>
            
            <div id="tab-friends" class="hidden tab-transition p-4 space-y-6">
                <div class="bg-wa-accent/5 p-4 rounded-2xl border border-wa-accent/10">
                    <h4 class="text-xs font-bold text-wa-accent uppercase mb-3">Nutzer hinzufügen</h4>
                    <div class="flex gap-2">
                        <input id="friendSearchInput" type="text" class="flex-1 bg-white dark:bg-wa-panel p-2 rounded-lg text-sm border dark:border-transparent outline-none" placeholder="ID einfügen">
                        <button onclick="window.sendRequest()" class="bg-wa-accent text-white p-2 rounded-lg"><i data-lucide="user-plus" class="w-5 h-5"></i></button>
                    </div>
                </div>
                <div id="pendingContainer" class="hidden">
                    <h4 class="text-[10px] font-black text-orange-500 uppercase tracking-widest mb-2">Offene Anfragen</h4>
                    <div id="pendingList" class="space-y-2"></div>
                </div>
                <div>
                    <h4 class="text-[10px] font-black text-gray-500 uppercase tracking-widest mb-2">Deine Kontakte</h4>
                    <div id="contactsList" class="space-y-1"></div>
                </div>
            </div>

            <div id="tab-status" class="hidden tab-transition p-4">
                <div class="flex items-center gap-4 mb-8 cursor-pointer group" onclick="document.getElementById('stIn').click()">
                    <div class="relative">
                        <div class="w-14 h-14 rounded-full bg-gray-200 dark:bg-gray-800 flex items-center justify-center overflow-hidden border-2 border-dashed border-gray-400">
                            <i data-lucide="camera" class="text-gray-400 group-hover:scale-110 transition"></i>
                        </div>
                        <div class="absolute -bottom-1 -right-1 bg-wa-accent rounded-full p-1 border-4 border-white dark:border-wa-dark">
                            <i data-lucide="plus" class="w-3 h-3 text-white"></i>
                        </div>
                    </div>
                    <div><h3 class="font-bold">Mein Status</h3><p class="text-xs text-gray-500">Bild für 24h teilen</p></div>
                    <input type="file" id="stIn" class="hidden" accept="image/*" onchange="window.upStatus()">
                </div>
                <div id="globalStatusList" class="space-y-5"></div>
            </div>

            <div id="tab-settings" class="hidden tab-transition p-6 space-y-8">
                <div class="text-center">
                    <div class="w-24 h-24 rounded-full bg-wa-accent mx-auto flex items-center justify-center text-4xl font-black text-white shadow-xl mb-4" id="setAvatar">?</div>
                    <h2 class="text-xl font-bold" id="setNick">---</h2>
                </div>
                <div class="space-y-4">
                    <div class="bg-gray-50 dark:bg-wa-panel p-4 rounded-xl">
                        <label class="text-[10px] font-bold text-wa-accent uppercase">Nickname ändern</label>
                        <div class="flex gap-2 mt-1">
                            <input id="nickIn" type="text" class="flex-1 bg-transparent border-b border-gray-300 dark:border-gray-600 outline-none pb-1">
                            <button onclick="window.saveProfile()" class="text-wa-accent font-bold text-xs">OK</button>
                        </div>
                    </div>
                    <div class="bg-gray-50 dark:bg-wa-panel p-4 rounded-xl">
                        <label class="text-[10px] font-bold text-gray-500 uppercase">Deine Account ID</label>
                        <p id="myFullId" onclick="window.copyMyId()" class="text-xs font-mono mt-1 text-wa-accent truncate cursor-pointer hover:underline"></p>
                    </div>
                    <div class="bg-gray-100 dark:bg-wa-panel p-4 rounded-xl border border-wa-accent/20">
                        <label class="text-[10px] font-bold text-wa-accent uppercase">Sicherheits-Passwort</label>
                        <p id="myPasswordDisp" class="text-lg font-mono font-bold tracking-widest mt-1">********</p>
                        <p class="text-[9px] text-gray-400 mt-2 italic">Speichere dieses Passwort für Logins auf anderen Geräten!</p>
                    </div>
                    <button onclick="window.toggleModal('switchAccountModal')" class="w-full py-4 rounded-2xl bg-gray-200 dark:bg-gray-700 font-bold text-sm hover:scale-[1.02] transition">Hast du bereits ein Konto? Login</button>
                </div>
            </div>
        </div>
        
        <nav class="md:hidden h-[70px] bg-white dark:bg-wa-panel border-t dark:border-gray-800 flex justify-around items-center shrink-0">
            <button onclick="window.switchTab('chats')" class="flex flex-col items-center"><i data-lucide="message-square"></i><span class="text-[10px]">Chats</span></button>
            <button onclick="window.switchTab('status')" class="flex flex-col items-center"><i data-lucide="circle-dashed"></i><span class="text-[10px]">Status</span></button>
            <button onclick="window.switchTab('friends')" class="flex flex-col items-center"><i data-lucide="users"></i><span class="text-[10px]">Kontakte</span></button>
            <button onclick="window.switchTab('settings')" class="flex flex-col items-center"><i data-lucide="settings"></i><span class="text-[10px]">Profil</span></button>
        </nav>
    </aside>

    <main id="chatArea" class="hidden md:flex flex-1 flex-col h-full relative overflow-hidden bg-[#efeae2] dark:bg-[#0b141a]">
        <div class="absolute inset-0 opacity-[0.05] pointer-events-none" style="background-image: url('https://user-images.githubusercontent.com/15075759/28719144-86dc0f70-73b1-11e7-911d-60d70fcded21.png');"></div>

        <header id="chatHeader" class="hidden h-[64px] bg-wa-light dark:bg-wa-panel items-center justify-between px-4 shrink-0 z-10 shadow-md">
            <div class="flex items-center gap-3">
                <button onclick="window.closeChatMobile()" class="md:hidden p-2"><i data-lucide="arrow-left"></i></button>
                <div id="activeAvatar" class="w-10 h-10 rounded-full bg-wa-accent flex items-center justify-center text-white font-bold">?</div>
                <div class="cursor-pointer" onclick="window.toggleModal('infoModal')">
                    <h2 id="activeTitle" class="font-bold text-sm">Chat</h2>
                    <p id="activeSub" class="text-[10px] text-wa-accent font-bold">Online</p>
                </div>
            </div>
            <div class="flex items-center gap-1">
                <button onclick="window.initiateCall()" class="p-2.5 text-wa-accent"><i data-lucide="phone" class="w-5 h-5"></i></button>
                <button onclick="window.toggleModal('infoModal')" class="p-2.5 text-gray-500"><i data-lucide="more-vertical" class="w-5 h-5"></i></button>
            </div>
        </header>

        <div id="messagesList" class="flex-1 overflow-y-auto p-4 md:px-10 space-y-2 relative z-10 custom-scrollbar flex flex-col"></div>

        <footer id="chatInputArea" class="hidden p-3 bg-wa-light dark:bg-wa-panel items-end gap-2 shrink-0 z-10">
            <div id="attachmentPreview" class="hidden absolute bottom-20 left-4 p-2 bg-white dark:bg-wa-panel rounded-xl shadow-2xl border-2 border-wa-accent">
                <img id="prevImg" class="w-32 h-32 object-cover rounded-lg">
                <button onclick="window.clearAttachment()" class="absolute -top-2 -right-2 bg-red-500 text-white rounded-full p-1"><i data-lucide="x" class="w-3 h-3"></i></button>
            </div>
            <button onclick="document.getElementById('imgIn').click()" class="p-2.5 text-gray-500"><i data-lucide="paperclip"></i></button>
            <input type="file" id="imgIn" class="hidden" accept="image/*" onchange="window.handleFile()">
            <div class="flex-1 bg-white dark:bg-wa-dark rounded-xl px-4 py-2 min-h-[45px] shadow-sm">
                <textarea id="msgIn" placeholder="Nachricht..." class="w-full bg-transparent outline-none text-sm dark:text-white resize-none py-1" rows="1"></textarea>
            </div>
            <button id="sendBtn" onclick="window.sendMsg()" class="bg-wa-accent text-white p-3 rounded-xl"><i data-lucide="send-horizontal"></i></button>
        </footer>
    </main>

    <div id="switchAccountModal" class="hidden fixed inset-0 z-[300] glass-modal flex items-center justify-center p-4">
        <div class="bg-white dark:bg-wa-panel w-full max-w-sm rounded-[30px] p-8 shadow-2xl">
            <h3 class="text-2xl font-black mb-6">Konto wechseln</h3>
            <div class="space-y-4">
                <div class="space-y-1">
                    <label class="text-[10px] font-bold text-gray-400 uppercase ml-2">Deine ID</label>
                    <input id="acc_id" type="text" placeholder="ID eingeben" class="w-full bg-gray-100 dark:bg-wa-dark p-4 rounded-2xl outline-none font-mono text-sm">
                </div>
                <div class="space-y-1">
                    <label class="text-[10px] font-bold text-gray-400 uppercase ml-2">8-stelliges Passwort</label>
                    <input id="acc_pass" type="text" placeholder="Passwort eingeben" class="w-full bg-gray-100 dark:bg-wa-dark p-4 rounded-2xl outline-none font-mono text-sm">
                </div>
            </div>
            <div class="flex gap-3 mt-8">
                <button onclick="window.toggleModal('switchAccountModal')" class="flex-1 py-3 text-gray-500 font-bold">Abbruch</button>
                <button onclick="window.performAccountSwitch()" class="flex-1 bg-wa-accent py-3 rounded-2xl text-white font-bold">Einloggen</button>
            </div>
        </div>
    </div>

    <div id="newGroupModal" class="hidden fixed inset-0 z-[100] glass-modal flex items-center justify-center p-4">
        <div class="bg-white dark:bg-wa-panel w-full max-w-xs rounded-3xl p-6">
            <h3 class="text-xl font-bold mb-4">Gruppe erstellen</h3>
            <input id="ng_name" type="text" placeholder="Name" class="w-full bg-gray-100 dark:bg-wa-dark p-3 rounded-xl mb-3 outline-none">
            <input id="ng_pass" type="password" placeholder="Passwort (optional)" class="w-full bg-gray-100 dark:bg-wa-dark p-3 rounded-xl mb-4 outline-none">
            <button onclick="window.createGrp()" class="w-full bg-wa-accent text-white py-3 rounded-xl font-bold">Erstellen</button>
            <button onclick="window.toggleModal('newGroupModal')" class="w-full mt-2 text-gray-400 text-sm">Abbrechen</button>
        </div>
    </div>

    <div id="joinGroupModal" class="hidden fixed inset-0 z-[100] glass-modal flex items-center justify-center p-4">
        <div class="bg-white dark:bg-wa-panel w-full max-w-xs rounded-3xl p-6">
            <h3 class="text-xl font-bold mb-4">Gruppe beitreten</h3>
            <input id="jg_id" type="text" placeholder="GRP-ID" class="w-full bg-gray-100 dark:bg-wa-dark p-3 rounded-xl mb-3 outline-none font-mono text-center">
            <input id="jg_pass" type="password" placeholder="Passwort" class="w-full bg-gray-100 dark:bg-wa-dark p-3 rounded-xl mb-4 outline-none text-center">
            <button onclick="window.joinGrp()" class="w-full bg-wa-accent text-white py-3 rounded-xl font-bold">Beitreten</button>
            <button onclick="window.toggleModal('joinGroupModal')" class="w-full mt-2 text-gray-400 text-sm">Abbrechen</button>
        </div>
    </div>

    <div id="infoModal" class="hidden fixed inset-0 z-[100] glass-modal flex items-center justify-center p-4">
        <div class="bg-white dark:bg-wa-panel w-full max-w-sm rounded-[32px] overflow-hidden">
            <div class="h-24 bg-wa-accent flex items-center px-6 text-white">
                <h3 id="infoTit" class="text-xl font-black truncate">Info</h3>
            </div>
            <div class="p-6">
                <p id="infoBadge" class="text-[10px] text-gray-400 font-mono mb-4">ID: ---</p>
                <div id="infoMemberList" class="space-y-2 max-h-40 overflow-y-auto mb-6"></div>
                <button id="leaveBtn" onclick="window.leaveChatAction()" class="w-full py-4 rounded-2xl border-2 border-red-500 text-red-500 font-bold">Chat verlassen</button>
                <button onclick="window.toggleModal('infoModal')" class="w-full mt-4 text-gray-400 font-bold">Schließen</button>
            </div>
        </div>
    </div>

    <script type="module">
        import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
        import { getFirestore, collection, addDoc, query, orderBy, onSnapshot, serverTimestamp, doc, updateDoc, getDoc, setDoc, arrayUnion, arrayRemove, where, deleteDoc } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-firestore.js";
        import { getAuth, signInAnonymously, onAuthStateChanged } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-auth.js";

        const firebaseConfig = {
            apiKey: "AIzaSyBC_O4jysYWBVuSeLJ6-ylx8OXrHBKQjfc",
            authDomain: "shadowchat-e94f0.firebaseapp.com",
            projectId: "shadowchat-e94f0",
            storageBucket: "shadowchat-e94f0.firebasestorage.app",
            messagingSenderId: "555331269217",
            appId: "1:555331269217:web:872ef3fddca93db0957020"
        };

        const app = initializeApp(firebaseConfig);
        const db = getFirestore(app);
        const auth = getAuth(app);
        const IMGBB_KEY = "bd8f03312d551a4e7c4973f6af975396";

        let currentUser = null;
        let activeChatId = null;
        let myNick = "Gast";
        let myPassword = "";
        let messageUnsub = null;

        lucide.createIcons();

        // PASSWORT GENERATOR (8 Zeichen: A-Z, a-z, 0-9, !)
        function generatePass() {
            const chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!?";
            let res = "";
            for(let i=0; i<8; i++) res += chars.charAt(Math.floor(Math.random() * chars.length));
            return res;
        }

        // --- AUTH & LOGIN ---
        signInAnonymously(auth);
        onAuthStateChanged(auth, async (user) => {
            if(user) {
                currentUser = user;
                const userRef = doc(db, "users", user.uid);
                const snap = await getDoc(userRef);
                
                if(!snap.exists()) {
                    myNick = "User-" + Math.floor(Math.random()*9000+1000);
                    myPassword = generatePass();
                    await setDoc(userRef, { uid: user.uid, nickname: myNick, password: myPassword, friends: [], incoming: [], lastSeen: serverTimestamp() });
                } else {
                    const data = snap.data();
                    myNick = data.nickname;
                    myPassword = data.password || generatePass();
                    if(!data.password) await updateDoc(userRef, { password: myPassword });
                }
                
                initUI();
                loadChats();
                loadFriends();
                loadStatus();
            }
        });

        function initUI() {
            document.getElementById('myAvatar').innerText = myNick[0].toUpperCase();
            document.getElementById('setAvatar').innerText = myNick[0].toUpperCase();
            document.getElementById('myDisplayNick').innerText = myNick;
            document.getElementById('setNick').innerText = myNick;
            document.getElementById('nickIn').value = myNick;
            document.getElementById('myFullId').innerText = currentUser.uid;
            document.getElementById('myPasswordDisp').innerText = myPassword;
            lucide.createIcons();
        }

        // --- ACCOUNT SWITCH ---
        window.performAccountSwitch = async () => {
            const id = document.getElementById('acc_id').value.trim();
            const pass = document.getElementById('acc_pass').value.trim();
            if(!id || !pass) return alert("Bitte ID und Passwort eingeben!");

            const snap = await getDoc(doc(db, "users", id));
            if(snap.exists() && snap.data().password === pass) {
                // Hier tricksen wir: Wir nutzen die neue ID für alle Abfragen
                currentUser = { uid: id };
                myNick = snap.data().nickname;
                myPassword = snap.data().password;
                initUI();
                loadChats();
                loadFriends();
                window.toggleModal('switchAccountModal');
                alert("Erfolgreich eingeloggt als " + myNick);
            } else {
                alert("ID oder Passwort falsch!");
            }
        };

        // --- MESSAGING ---
        window.sendMsg = async () => {
            const inp = document.getElementById('msgIn');
            const file = document.getElementById('imgIn').files[0];
            const txt = inp.value.trim();
            if(!txt && !file) return;

            let url = null;
            if(file) {
                const fd = new FormData(); fd.append("image", file);
                const res = await fetch(`https://api.imgbb.com/1/upload?key=${IMGBB_KEY}`, { method: "POST", body: fd });
                const d = await res.json(); url = d.data.url;
            }

            await addDoc(collection(db, `chats/${activeChatId}/messages`), { 
                text: txt, img: url, senderId: currentUser.uid, senderName: myNick, timestamp: serverTimestamp(), edited: false 
            });
            await updateDoc(doc(db, "chats", activeChatId), { lastActivity: serverTimestamp() });
            
            inp.value = ""; 
            window.clearAttachment();
        };

        window.editMessage = async (msgId, oldTxt) => {
            const newTxt = prompt("Nachricht bearbeiten:", oldTxt);
            if(newTxt !== null && newTxt !== oldTxt) {
                await updateDoc(doc(db, `chats/${activeChatId}/messages`, msgId), { text: newTxt, edited: true });
            }
        };

        window.openChat = (id, name) => {
            activeChatId = id;
            document.getElementById('chatHeader').classList.replace('hidden', 'flex');
            document.getElementById('chatInputArea').classList.replace('hidden', 'flex');
            document.getElementById('activeTitle').innerText = name;
            
            if(window.innerWidth < 768) {
                document.getElementById('mainSidebar').classList.add('hidden');
                document.getElementById('chatArea').classList.replace('hidden', 'flex');
            }

            if(messageUnsub) messageUnsub();
            const q = query(collection(db, `chats/${id}/messages`), orderBy("timestamp", "asc"));
            messageUnsub = onSnapshot(q, snap => {
                const list = document.getElementById('messagesList');
                list.innerHTML = "";
                snap.forEach(docSnap => {
                    const m = docSnap.data();
                    const isMe = m.senderId === currentUser.uid;
                    const div = document.createElement('div');
                    div.className = `flex ${isMe ? 'justify-end' : 'justify-start'} mb-1 msg-anim`;
                    div.innerHTML = `
                        <div class="max-w-[80%] p-2 rounded-xl shadow-sm ${isMe ? 'bg-wa-bubbleOut dark:bg-wa-bubbleDarkOut' : 'bg-white dark:bg-wa-bubbleDarkIn'}">
                            ${!isMe ? `<p class="text-[10px] font-bold text-wa-accent mb-1">${m.senderName}</p>` : ''}
                            ${m.img ? `<img src="${m.img}" class="rounded-lg mb-1 max-w-full">` : ''}
                            <div class="flex items-end gap-2">
                                <p class="text-[14px] dark:text-gray-100">${m.text || ''}</p>
                                ${m.edited ? '<span class="text-[9px] opacity-40 italic">bearbeitet</span>' : ''}
                                ${isMe ? `<button onclick="window.editMessage('${docSnap.id}', '${m.text}')" class="opacity-20 hover:opacity-100"><i data-lucide="pencil" class="w-3 h-3"></i></button>` : ''}
                            </div>
                        </div>`;
                    list.appendChild(div);
                });
                list.scrollTop = list.scrollHeight;
                lucide.createIcons();
            });
            updateInfoModal(id, name);
        };

        // --- GROUPS & LEAVE FIX ---
        window.createGrp = async () => {
            const n = document.getElementById('ng_name').value.trim();
            const p = document.getElementById('ng_pass').value.trim();
            if(!n) return;
            const gid = "GRP-" + Math.random().toString(36).substring(2,9).toUpperCase();
            await setDoc(doc(db, "chats", gid), { type: 'group', name: n, password: p || null, members: [currentUser.uid], lastActivity: serverTimestamp() });
            window.toggleModal('newGroupModal');
            window.openChat(gid, n);
        };

        window.joinGrp = async () => {
            const id = document.getElementById('jg_id').value.toUpperCase().trim();
            const p = document.getElementById('jg_pass').value.trim();
            const s = await getDoc(doc(db, "chats", id));
            if(s.exists()){
                if(s.data().password && s.data().password !== p) return alert("Falsches Passwort");
                await updateDoc(doc(db, "chats", id), { members: arrayUnion(currentUser.uid) });
                window.toggleModal('joinGroupModal');
                window.openChat(id, s.data().name);
            }
        };

        window.leaveChatAction = async () => {
            if(!activeChatId) return;
            if(confirm("Diesen Chat wirklich verlassen?")) {
                const ref = doc(db, "chats", activeChatId);
                const snap = await getDoc(ref);
                if(snap.exists()) {
                    if(snap.data().type === 'group') {
                        await updateDoc(ref, { members: arrayRemove(currentUser.uid) });
                    } else {
                        await deleteDoc(ref); // Privatchat löschen
                    }
                    window.toggleModal('infoModal');
                    window.closeChatMobile();
                }
            }
        };

        // --- FRIENDS & REQUESTS ---
        window.sendRequest = async () => {
            const id = document.getElementById('friendSearchInput').value.trim();
            if(!id || id === currentUser.uid) return alert("Ungültige ID");
            const tRef = doc(db, "users", id);
            const tSnap = await getDoc(tRef);
            if(tSnap.exists()) {
                await updateDoc(tRef, { incoming: arrayUnion(currentUser.uid) });
                alert("Anfrage gesendet!");
            } else alert("User nicht gefunden.");
        };

        function loadFriends() {
            onSnapshot(doc(db, "users", currentUser.uid), async (snap) => {
                if(!snap.exists()) return;
                const data = snap.data();
                
                // Anfragen
                const pCont = document.getElementById('pendingContainer');
                const pList = document.getElementById('pendingList');
                pList.innerHTML = "";
                if(data.incoming?.length > 0) {
                    pCont.classList.remove('hidden');
                    data.incoming.forEach(async (fid) => {
                        const fSnap = await getDoc(doc(db, "users", fid));
                        const div = document.createElement('div');
                        div.className = "flex items-center justify-between bg-white dark:bg-wa-panel p-3 rounded-xl shadow-sm";
                        div.innerHTML = `<span class="text-sm font-bold">${fSnap.data().nickname}</span>
                            <div class="flex gap-2">
                                <button onclick="window.acceptF('${fid}')" class="bg-wa-accent text-white p-1.5 rounded-lg"><i data-lucide="check" class="w-4 h-4"></i></button>
                                <button onclick="window.rejectF('${fid}')" class="bg-red-500 text-white p-1.5 rounded-lg"><i data-lucide="x" class="w-4 h-4"></i></button>
                            </div>`;
                        pList.appendChild(div);
                        lucide.createIcons();
                    });
                } else pCont.classList.add('hidden');

                // Kontakte
                const cList = document.getElementById('contactsList');
                cList.innerHTML = "";
                data.friends?.forEach(async (fid) => {
                    const fSnap = await getDoc(doc(db, "users", fid));
                    const div = document.createElement('div');
                    div.className = "flex items-center gap-3 p-3 hover:bg-gray-100 dark:hover:bg-wa-panel rounded-xl cursor-pointer";
                    div.innerHTML = `<div class="w-10 h-10 rounded-full bg-gray-300 flex items-center justify-center font-bold">${fSnap.data().nickname[0]}</div>
                                     <span class="text-sm font-medium">${fSnap.data().nickname}</span>`;
                    div.onclick = () => window.startPrivateChat(fid, fSnap.data().nickname);
                    cList.appendChild(div);
                });
            });
        }

        window.acceptF = async (fid) => {
            const myRef = doc(db, "users", currentUser.uid);
            const hisRef = doc(db, "users", fid);
            await updateDoc(myRef, { incoming: arrayRemove(fid), friends: arrayUnion(fid) });
            await updateDoc(hisRef, { friends: arrayUnion(currentUser.uid) });
        };

        window.startPrivateChat = async (fid, fname) => {
            const cid = currentUser.uid < fid ? currentUser.uid + "_" + fid : fid + "_" + currentUser.uid;
            await setDoc(doc(db, "chats", cid), { 
                type: 'private', 
                members: [currentUser.uid, fid], 
                name: fname, 
                lastActivity: serverTimestamp() 
            }, { merge: true });
            window.switchTab('chats');
            window.openChat(cid, fname);
        };

        // --- STATUS ---
        window.upStatus = async () => {
            const file = document.getElementById('stIn').files[0];
            if(!file) return;
            const fd = new FormData(); fd.append("image", file);
            const res = await fetch(`https://api.imgbb.com/1/upload?key=${IMGBB_KEY}`, { method: "POST", body: fd });
            const d = await res.json();
            await addDoc(collection(db, "status"), { 
                uid: currentUser.uid, 
                nick: myNick, 
                img: d.data.url, 
                time: serverTimestamp(), 
                expires: Date.now() + 86400000 
            });
            alert("Status gepostet!");
        };

        function loadStatus() {
            const q = query(collection(db, "status"), orderBy("time", "desc"));
            onSnapshot(q, snap => {
                const cont = document.getElementById('globalStatusList');
                cont.innerHTML = "";
                snap.forEach(doc => {
                    const s = doc.data();
                    if(s.expires > Date.now()) {
                        const div = document.createElement('div');
                        div.className = "flex items-center gap-3 p-1";
                        div.innerHTML = `
                            <div class="p-[2px] rounded-full border-2 border-wa-accent">
                                <img src="${s.img}" class="w-12 h-12 rounded-full object-cover">
                            </div>
                            <div><p class="font-bold text-sm">${s.nick}</p><p class="text-[10px] text-gray-500">Gerade eben</p></div>`;
                        cont.appendChild(div);
                    }
                });
            });
        }

        // --- UTILS ---
        function loadChats() {
            const q = query(collection(db, "chats"), orderBy("lastActivity", "desc"));
            onSnapshot(q, snap => {
                const cont = document.getElementById('chatListContainer');
                cont.innerHTML = "";
                snap.forEach(d => {
                    const c = d.data();
                    if(c.members?.includes(currentUser.uid)) {
                        const name = c.name || "Privat Chat";
                        const div = document.createElement('div');
                        div.className = "flex items-center gap-4 p-4 hover:bg-gray-100 dark:hover:bg-wa-panel cursor-pointer border-l-4 border-transparent hover:border-wa-accent";
                        div.innerHTML = `<div class="w-12 h-12 rounded-full bg-wa-accent flex items-center justify-center text-white font-black">${name[0]}</div>
                                         <div class="flex-1 truncate"><h4 class="font-bold text-sm">${name}</h4><p class="text-xs text-gray-500 truncate">${c.type === 'group' ? 'Gruppe' : 'Chat'}</p></div>`;
                        div.onclick = () => window.openChat(d.id, name);
                        cont.appendChild(div);
                    }
                });
            });
        }

        function updateInfoModal(id, name) {
            document.getElementById('infoTit').innerText = name;
            document.getElementById('infoBadge').innerText = "ID: " + id;
            const ml = document.getElementById('infoMemberList'); ml.innerHTML = "";
            getDoc(doc(db, "chats", id)).then(s => {
                s.data().members.forEach(async mid => {
                    const u = await getDoc(doc(db, "users", mid));
                    const d = document.createElement('div');
                    d.className = "p-3 bg-gray-50 dark:bg-wa-dark rounded-xl text-xs font-bold flex justify-between";
                    d.innerHTML = `<span>${u.data().nickname}</span> <span class="text-wa-accent">Aktiv</span>`;
                    ml.appendChild(d);
                });
            });
        }

        window.toggleModal = (id) => document.getElementById(id).classList.toggle('hidden');
        window.switchTab = (t) => {
            ['chats', 'friends', 'status', 'settings'].forEach(x => document.getElementById('tab-'+x).classList.add('hidden'));
            document.getElementById('tab-'+t).classList.remove('hidden');
            lucide.createIcons();
        };
        window.toggleTheme = () => document.documentElement.classList.toggle('dark');
        window.copyMyId = () => { navigator.clipboard.writeText(currentUser.uid); alert("ID kopiert!"); };
        window.closeChatMobile = () => { document.getElementById('chatArea').classList.add('hidden'); document.getElementById('mainSidebar').classList.remove('hidden'); };
        window.handleFile = () => {
            const f = document.getElementById('imgIn').files[0];
            if(f) {
                document.getElementById('prevImg').src = URL.createObjectURL(f);
                document.getElementById('attachmentPreview').classList.remove('hidden');
            }
        };
        window.clearAttachment = () => {
            document.getElementById('imgIn').value = "";
            document.getElementById('attachmentPreview').classList.add('hidden');
        };
        window.saveProfile = async () => {
            const n = document.getElementById('nickIn').value.trim();
            if(n) { await updateDoc(doc(db, "users", currentUser.uid), { nickname: n }); location.reload(); }
        };

    </script>
</body>
</html>
