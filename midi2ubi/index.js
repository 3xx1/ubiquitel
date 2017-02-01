// num of ch
var CHANNEL = 1;

// const
var LOOP_UNIT = 256;
var LINEBREAK = ',\n';
var LASTLINES = 'addloop,256,1' + LINEBREAK + 'sendNotes' + LINEBREAK;

// dependencies
var fs = require('fs');
var parseMidi = require('midi-file').parseMidi;


var input = fs.readFileSync('input/sample.mid');
var parsed = parseMidi(input);
var notes = parsed.tracks[0];

var wholeNote = parsed.header.ticksPerBeat * 4;

var timeline = 0;
var outputText = 'reset' + LINEBREAK;

for (var i=0; i<notes.length; i++) {
  if (notes[i].velocity) {
    timeline += notes[i].deltaTime;
    if (notes[i].type == 'noteOn') {
      var timing = Math.floor(timeline*(LOOP_UNIT/wholeNote));
      var velocity = Math.floor((notes[i].velocity/127)*10) * 20;
      outputText += ('add,' + timing + ',' + velocity + LINEBREAK);
    }
  }
}
outputText += LASTLINES

fs.writeFile('hello.txt', outputText, function(err) {
  if (err) console.log(err);
});
