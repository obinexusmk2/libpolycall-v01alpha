const test = require('node:test');
const assert = require('node:assert/strict');

const {
  loadTransitionScenarios,
  loadRetryScenarios,
  loadAckTimelineScenarios,
} = require('./fixtureLoader');

const evaluateRetry = (maxRetries, attempts) => {
  let retries = 0;
  for (const attempt of attempts) {
    if (attempt === 'YES' || attempt === 'NO') {
      return { finalState: attempt, retryCount: retries };
    }
    if (retries < maxRetries) {
      retries += 1;
    }
  }
  return { finalState: 'NO', retryCount: maxRetries };
};

const evaluateAck = (deadlineMs, events) => {
  const ack = events.find((event) => event.name === 'ack_received');
  if (!ack) return 'MISSING';
  return ack.at_ms <= deadlineMs ? 'ON_TIME' : 'LATE';
};

test('transition parity from shared fixtures', () => {
  for (const scenario of loadTransitionScenarios()) {
    assert.equal(scenario.decision, scenario.expected_state, scenario.id);
  }
});

test('retry parity from shared fixtures', () => {
  for (const scenario of loadRetryScenarios()) {
    const actual = evaluateRetry(scenario.max_retries, scenario.attempts);
    assert.equal(actual.finalState, scenario.expected_final, scenario.id);
    assert.equal(actual.retryCount, scenario.expected_retry_count, scenario.id);
  }
});

test('ack timeline parity from shared fixtures', () => {
  for (const scenario of loadAckTimelineScenarios()) {
    assert.equal(
      evaluateAck(scenario.ack_deadline_ms, scenario.events),
      scenario.expected_ack_status,
      scenario.id,
    );
  }
});
