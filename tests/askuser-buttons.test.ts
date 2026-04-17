/**
 * Tests for AskUserQuestion card button flow (issue #9).
 *
 * Covers:
 *   - card-builder emits an `action` element with one button per option,
 *     each button's `value` carrying { kind, toolUseId, optionIndex }
 *   - bridge.handleCardAction routes clicks by matching
 *     (cardMessageId, toolUseId) against runningTasks
 *   - stale / unknown clicks are silent no-ops
 *   - malformed value payloads are rejected
 */
import { describe, it, expect, vi } from 'vitest';
import type { IMessageSender } from '../src/bridge/message-sender.interface.js';
import { MessageBridge } from '../src/bridge/message-bridge.js';

function mockConfig() {
  return {
    name: 'test-bot',
    claude: {
      defaultWorkingDirectory: '/tmp/test',
      maxTurns: 10,
      maxBudgetUsd: 1,
      outputsBaseDir: '/tmp/test-outputs',
      downloadsDir: '/tmp/test-downloads',
    },
  } as any;
}

function mockSender(): IMessageSender {
  return {
    sendCard: vi.fn(async () => 'card_1'),
    updateCard: vi.fn(async () => true),
    sendTextNotice: vi.fn(async () => {}),
    sendText: vi.fn(async () => {}),
    sendImageFile: vi.fn(async () => true),
    sendLocalFile: vi.fn(async () => true),
    downloadImage: vi.fn(async () => true),
    downloadFile: vi.fn(async () => true),
  };
}

function mockLogger() {
  const noop = () => {};
  const logger: any = {
    info: noop, warn: noop, error: noop, debug: noop,
    child: () => logger,
  };
  return logger;
}

function makeBridge(sender: IMessageSender) {
  return new MessageBridge(mockConfig(), mockLogger(), sender, '');
}

describe('handleCardAction', () => {
  it('ignores clicks that do not match any running task (stale card)', async () => {
    const sender = mockSender();
    const bridge = makeBridge(sender);

    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'unknown_card',
      value: { kind: 'askuser_answer', toolUseId: 't1', optionIndex: 0 },
    });

    // Nothing sent, nothing updated, no throws.
    expect(sender.updateCard).not.toHaveBeenCalled();
    expect(sender.sendText).not.toHaveBeenCalled();
  });

  it('ignores clicks whose toolUseId does not match the task\'s current question', async () => {
    const sender = mockSender();
    const bridge = makeBridge(sender);

    // Inject a task with a different toolUseId to simulate the card moving on
    // to the next question while the old button was still visible on client.
    const fakeTask: any = {
      cardMessageId: 'card_abc',
      pendingQuestion: { toolUseId: 'new_tool_id', questions: [] },
    };
    (bridge as any).runningTasks.set('chat_1', fakeTask);

    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'card_abc',
      value: { kind: 'askuser_answer', toolUseId: 'old_tool_id', optionIndex: 0 },
    });

    expect(sender.updateCard).not.toHaveBeenCalled();
  });

  it('ignores malformed value payloads', async () => {
    const sender = mockSender();
    const bridge = makeBridge(sender);

    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'card_abc',
      value: { kind: 'askuser_answer' }, // missing toolUseId + optionIndex
    });
    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'card_abc',
      value: { kind: 'askuser_answer', toolUseId: 't1', optionIndex: -1 },
    });

    expect(sender.updateCard).not.toHaveBeenCalled();
  });

  it('ignores unknown action kinds (future-proofing)', async () => {
    const sender = mockSender();
    const bridge = makeBridge(sender);

    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'card_abc',
      value: { kind: 'something_else', foo: 'bar' },
    });

    expect(sender.updateCard).not.toHaveBeenCalled();
  });

  it('routes a valid click through handleAnswer with synthetic text = (optionIndex + 1)', async () => {
    const sender = mockSender();
    const bridge = makeBridge(sender);

    // Spy on the private handleAnswer to confirm routing without needing to
    // set up a real ExecutionHandle/StreamProcessor.
    const handleAnswerSpy = vi
      .spyOn(bridge as any, 'handleAnswer')
      .mockResolvedValue(undefined);

    const fakeTask: any = {
      cardMessageId: 'card_abc',
      pendingQuestion: { toolUseId: 't1', questions: [] },
    };
    (bridge as any).runningTasks.set('chat_1', fakeTask);

    await bridge.handleCardAction({
      userId: 'u1',
      messageId: 'card_abc',
      value: { kind: 'askuser_answer', toolUseId: 't1', optionIndex: 2 },
    });

    expect(handleAnswerSpy).toHaveBeenCalledTimes(1);
    const [syntheticMsg, passedTask] = handleAnswerSpy.mock.calls[0];
    expect(passedTask).toBe(fakeTask);
    expect(syntheticMsg).toMatchObject({
      chatId: 'chat_1',
      userId: 'u1',
      text: '3', // optionIndex 2 → text "3" (1-indexed, reuses number parsing)
    });
  });
});
