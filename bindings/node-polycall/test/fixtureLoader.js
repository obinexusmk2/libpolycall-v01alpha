const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..', '..', '..');
const FIXTURE_DIR = path.join(ROOT, 'fixtures', 'consensus');

function loadConsensusFixture(name) {
  const fixturePath = path.join(FIXTURE_DIR, `${name}.json`);
  return JSON.parse(fs.readFileSync(fixturePath, 'utf8'));
}

module.exports = {
  loadTransitionScenarios: () => loadConsensusFixture('state_transitions').scenarios,
  loadRetryScenarios: () => loadConsensusFixture('retries').scenarios,
  loadAckTimelineScenarios: () => loadConsensusFixture('ack_timelines').scenarios,
};
