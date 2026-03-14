import { type ReactNode, useState, useCallback } from 'react';
import { useNavigate } from 'react-router-dom';
import { useStore } from '../store';
import { useWebSocket } from '../hooks/useWebSocket';
import styles from './Layout.module.css';

/* ---- SVG Icons (inline, no external deps) ---- */

function IconChat() {
  return (
    <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M21 15a2 2 0 01-2 2H7l-4 4V5a2 2 0 012-2h14a2 2 0 012 2z" />
    </svg>
  );
}

function IconMemory() {
  return (
    <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M4 19.5A2.5 2.5 0 016.5 17H20" />
      <path d="M6.5 2H20v20H6.5A2.5 2.5 0 014 19.5v-15A2.5 2.5 0 016.5 2z" />
    </svg>
  );
}

function IconVoice() {
  return (
    <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M12 1a3 3 0 00-3 3v8a3 3 0 006 0V4a3 3 0 00-3-3z" />
      <path d="M19 10v2a7 7 0 01-14 0v-2" />
      <line x1="12" y1="19" x2="12" y2="23" />
      <line x1="8" y1="23" x2="16" y2="23" />
    </svg>
  );
}

function IconSettings() {
  return (
    <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <circle cx="12" cy="12" r="3" />
      <path d="M19.4 15a1.65 1.65 0 00.33 1.82l.06.06a2 2 0 010 2.83 2 2 0 01-2.83 0l-.06-.06a1.65 1.65 0 00-1.82-.33 1.65 1.65 0 00-1 1.51V21a2 2 0 01-4 0v-.09A1.65 1.65 0 009 19.4a1.65 1.65 0 00-1.82.33l-.06.06a2 2 0 01-2.83 0 2 2 0 010-2.83l.06-.06A1.65 1.65 0 004.68 15a1.65 1.65 0 00-1.51-1H3a2 2 0 010-4h.09A1.65 1.65 0 004.6 9a1.65 1.65 0 00-.33-1.82l-.06-.06a2 2 0 012.83-2.83l.06.06A1.65 1.65 0 009 4.68a1.65 1.65 0 001-1.51V3a2 2 0 014 0v.09a1.65 1.65 0 001 1.51 1.65 1.65 0 001.82-.33l.06-.06a2 2 0 012.83 2.83l-.06.06A1.65 1.65 0 0019.4 9a1.65 1.65 0 001.51 1H21a2 2 0 010 4h-.09a1.65 1.65 0 00-1.51 1z" />
    </svg>
  );
}

function IconPlus() {
  return (
    <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round">
      <line x1="12" y1="5" x2="12" y2="19" />
      <line x1="5" y1="12" x2="19" y2="12" />
    </svg>
  );
}

function IconX() {
  return (
    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round">
      <line x1="18" y1="6" x2="6" y2="18" />
      <line x1="6" y1="6" x2="18" y2="18" />
    </svg>
  );
}

function IconMenu() {
  return (
    <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round">
      <line x1="3" y1="6" x2="21" y2="6" />
      <line x1="3" y1="12" x2="21" y2="12" />
      <line x1="3" y1="18" x2="21" y2="18" />
    </svg>
  );
}

function IconSidebarLeft() {
  return (
    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <rect x="3" y="3" width="18" height="18" rx="2" ry="2" />
      <line x1="9" y1="3" x2="9" y2="21" />
    </svg>
  );
}

function IconChevron() {
  return (
    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <polyline points="6 9 12 15 18 9" />
    </svg>
  );
}

/* ---- Time formatting ---- */

function formatRelativeTime(ts: number): string {
  const diff = Date.now() - ts;
  const mins = Math.floor(diff / 60_000);
  if (mins < 1) return 'just now';
  if (mins < 60) return `${mins}m ago`;
  const hours = Math.floor(mins / 60);
  if (hours < 24) return `${hours}h ago`;
  const days = Math.floor(hours / 24);
  if (days < 7) return `${days}d ago`;
  return new Date(ts).toLocaleDateString();
}

/* ---- Component ---- */

interface LayoutProps {
  children: ReactNode;
}

export function Layout({ children }: LayoutProps) {
  const navigate = useNavigate();
  const activeView = useStore((s) => s.activeView);
  const setView = useStore((s) => s.setView);
  const sidebarOpen = useStore((s) => s.sidebarOpen);
  const toggleSidebar = useStore((s) => s.toggleSidebar);
  const setSidebarOpen = useStore((s) => s.setSidebarOpen);
  const connected = useStore((s) => s.connected);
  const bots = useStore((s) => s.bots);
  const activeBotName = useStore((s) => s.activeBotName);
  const setBot = useStore((s) => s.setBot);
  const sessions = useStore((s) => s.sessions);
  const activeSessionId = useStore((s) => s.activeSessionId);
  const setActiveSession = useStore((s) => s.setActiveSession);
  const createSession = useStore((s) => s.createSession);
  const deleteSession = useStore((s) => s.deleteSession);

  const [botDropdownOpen, setBotDropdownOpen] = useState(false);

  // Initialize WebSocket connection
  useWebSocket();

  const navItems = [
    { view: 'chat' as const, icon: <IconChat />, label: 'Chat', path: '/' },
    { view: 'memory' as const, icon: <IconMemory />, label: 'Memory', path: '/memory' },
    { view: 'voice' as const, icon: <IconVoice />, label: 'Voice', path: '/voice' },
    { view: 'settings' as const, icon: <IconSettings />, label: 'Settings', path: '/settings' },
  ];

  const handleNav = useCallback(
    (view: typeof activeView, path: string) => {
      setView(view);
      navigate(path);
      // Close sidebar on mobile
      if (window.innerWidth <= 768) {
        setSidebarOpen(false);
      }
    },
    [setView, navigate, setSidebarOpen],
  );

  const handleNewChat = useCallback(() => {
    createSession();
    setView('chat');
    navigate('/');
  }, [createSession, setView, navigate]);

  const handleSelectSession = useCallback(
    (id: string) => {
      setActiveSession(id);
      setView('chat');
      navigate('/');
      if (window.innerWidth <= 768) {
        setSidebarOpen(false);
      }
    },
    [setActiveSession, setView, navigate, setSidebarOpen],
  );

  const sortedSessions = Array.from(sessions.values()).sort(
    (a, b) => b.updatedAt - a.updatedAt,
  );

  const activeBot = bots.find((b) => b.name === activeBotName);

  return (
    <div className={styles.layout}>
      {/* Mobile hamburger */}
      {!sidebarOpen && (
        <button className={styles.hamburger} onClick={toggleSidebar}>
          <IconMenu />
        </button>
      )}

      {/* Mobile overlay */}
      {sidebarOpen && window.innerWidth <= 768 && (
        <div className={styles.overlay} onClick={() => setSidebarOpen(false)} />
      )}

      {/* Sidebar */}
      <aside
        className={`${styles.sidebar} ${!sidebarOpen ? styles.sidebarCollapsed : ''}`}
      >
        {/* Header */}
        <div className={styles.sidebarHeader}>
          <div className={styles.brand}>
            <div className={styles.brandIcon}>M</div>
            <span className={styles.brandName}>MetaBot</span>
          </div>
          <button className={styles.collapseBtn} onClick={toggleSidebar} title="Toggle sidebar">
            <IconSidebarLeft />
          </button>
        </div>

        {/* Navigation */}
        <nav className={styles.nav}>
          {navItems.map((item) => (
            <button
              key={item.view}
              className={`${styles.navItem} ${
                activeView === item.view ? styles.navItemActive : ''
              }`}
              onClick={() => handleNav(item.view, item.path)}
            >
              <span className={styles.navIcon}>{item.icon}</span>
              {item.label}
            </button>
          ))}
        </nav>

        <div className={styles.navDivider} />

        {/* Session List (only when chat is active) */}
        <div className={styles.sessionSection}>
          <div className={styles.sessionHeader}>
            <span className={styles.sessionTitle}>Conversations</span>
            <button
              className={styles.newChatBtn}
              onClick={handleNewChat}
              title="New Chat"
            >
              <IconPlus />
            </button>
          </div>

          <div className={styles.sessionList}>
            {sortedSessions.length === 0 ? (
              <div className={styles.emptySession}>No conversations yet</div>
            ) : (
              sortedSessions.map((session) => (
                <div
                  key={session.id}
                  className={`${styles.sessionItem} ${
                    activeSessionId === session.id ? styles.sessionItemActive : ''
                  }`}
                  onClick={() => handleSelectSession(session.id)}
                >
                  <div className={styles.sessionItemText}>
                    <div className={styles.sessionItemTitle}>{session.title}</div>
                    <div className={styles.sessionItemTime}>
                      {formatRelativeTime(session.updatedAt)}
                    </div>
                  </div>
                  <button
                    className={styles.sessionDeleteBtn}
                    onClick={(e) => {
                      e.stopPropagation();
                      deleteSession(session.id);
                    }}
                    title="Delete"
                  >
                    <IconX />
                  </button>
                </div>
              ))
            )}
          </div>
        </div>

        {/* Bot Selector */}
        <div className={styles.sidebarFooter}>
          <div className={styles.connStatus}>
            <span
              className={`${styles.connDot} ${
                connected ? styles.connDotOn : styles.connDotOff
              }`}
            />
            {connected ? 'Connected' : 'Disconnected'}
          </div>

          <div style={{ position: 'relative' }}>
            {botDropdownOpen && bots.length > 1 && (
              <div className={styles.botDropdown}>
                {bots.map((bot) => (
                  <button
                    key={bot.name}
                    className={`${styles.botOption} ${
                      activeBotName === bot.name ? styles.botOptionActive : ''
                    }`}
                    onClick={() => {
                      setBot(bot.name);
                      setBotDropdownOpen(false);
                    }}
                  >
                    <span
                      className={`${styles.botDot} ${styles.botDotOnline}`}
                    />
                    {bot.name}
                  </button>
                ))}
              </div>
            )}

            <div
              className={styles.botSelector}
              onClick={() => bots.length > 1 && setBotDropdownOpen(!botDropdownOpen)}
            >
              <span
                className={`${styles.botDot} ${
                  connected ? styles.botDotOnline : styles.botDotOffline
                }`}
              />
              <span className={styles.botSelectorName}>
                {activeBot?.name || 'No bot'}
              </span>
              {bots.length > 1 && (
                <span className={styles.botSelectorPlatform}>
                  <IconChevron />
                </span>
              )}
            </div>
          </div>
        </div>
      </aside>

      {/* Main Content */}
      <main className={styles.main}>{children}</main>
    </div>
  );
}
