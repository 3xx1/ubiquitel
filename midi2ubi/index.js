// num of ch
var CHANNEL = 2;

// const
var LOOP_UNIT = 256;
var LINEBREAK = ',\n';
var LASTLINES = 'addloop,256,1' + LINEBREAK + 'sendNotes' + LINEBREAK;
var BASEPATH = 'output/rhythm';

// dependencies
var path = require('path');
var fs = require('fs');
var parseMidi = require('midi-file').parseMidi;

// main function iteration
for (var index=0; index<CHANNEL; index++) {
  extractionMain(index);
}

function extractionMain(index) 
{
  var input = fs.readFileSync('input/ub' + index + '.mid');
  var parsed = parseMidi(input);
  var notes = parsed.tracks[0];
  var wholeNote = parsed.header.ticksPerBeat * 4;

  var timeline = 0;
  var outputText = 'reset' + LINEBREAK;
  var passage = 0;
  
  for (var i=0; i<notes.length; i++) {
    if (notes[i].velocity) {
      timeline += notes[i].deltaTime;
      if (timeline >= (passage+1)*wholeNote) {
        outputText += LASTLINES;
        var wholepath = BASEPATH + passage + '/' + passage + '.txt';
        fs.writeFile(path.join(BASEPATH+passage.toString(), 'rhythm'+passage.toString()+index.toString()+'.txt'), outputText, function(err) {
          if (err) console.log(err);
        });
        outputText = 'reset' + LINEBREAK;
        passage++;
      }
      if (notes[i].type == 'noteOn') {
        var timing = Math.floor(timeline*(LOOP_UNIT/wholeNote)) - passage*LOOP_UNIT;
        var velocity = Math.floor((notes[i].velocity/127)*10) * 20;
        outputText += ('add,' + timing + ',' + velocity + LINEBREAK);
      }
    }
  }
  outputText += LASTLINES;
  var wholepath = BASEPATH + passage + '/' + passage + '.txt';
  fs.writeFile(path.join(BASEPATH+passage.toString(), 'rhythm'+passage.toString()+index.toString()+'.txt'), outputText, function(err) {
    if (err) console.log(err);
  });
}
