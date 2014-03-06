#######################################################################
# Program that uses a Markov Chain to create a single-track MIDI file
#######################################################################

#import the midi util library and the random library
from midiutil.MidiFile import MIDIFile
import random
import sys

def addNotes(matrix, lookup):
  matrix[lookup['b5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['c6']] += 1
  matrix[lookup['c6'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['c#6']] += 1
  matrix[lookup['c#6'], lookup['b5']] += 1
  matrix[lookup['b5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['c6']] += 1
  matrix[lookup['c6'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['c#6']] += 1
  matrix[lookup['c#6'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1 
  matrix[lookup['e5'], lookup['e5']] += 1
  matrix[lookup['e5'], lookup['g5']] += 1 
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['e5']] += 1

  # Main Melody
  matrix[lookup['e5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['d#6']] += 1
  matrix[lookup['d#6'], lookup['d6']] += 1
  matrix[lookup['d6'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['a#5']] += 1
  matrix[lookup['a#5'], lookup['b5']] += 1
  matrix[lookup['b5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['a5']] += 1
  matrix[lookup['a5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['b5']] += 1
  matrix[lookup['b5'], lookup['e6']] += 1
  matrix[lookup['e6'], lookup['c#6']] += 1
  # Second Time Around
  matrix[lookup['e5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['d#6']] += 1
  matrix[lookup['d#6'], lookup['d6']] += 1
  matrix[lookup['d6'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['a#5']] += 1
  matrix[lookup['a#5'], lookup['b5']] += 1
  matrix[lookup['b5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['a5']] += 1
  matrix[lookup['a5'], lookup['g5']] += 1
  matrix[lookup['g5'], lookup['f#5']] += 1
  matrix[lookup['f#5'], lookup['b5']] += 1
  matrix[lookup['b5'], lookup['d#6']] += 1
  matrix[lookup['d#6'], lookup['e6']] += 1

def nextNote(currNote, lookup, matrix):
  rowSum = 0
  intermediateSum = 0

  for i in range(len(lookup)):
    rowSum += matrix[lookup[currNote], i]

  randCount = random.randrange(1, rowSum + 1)

  for i in range(len(lookup)):
    intermediateSum += matrix[lookup[currNote], i]
    if (intermediateSum >= randCount):
      for note, val in lookup.iteritems():
        if val == i:
          return note
  raise RuntimeError, "Impossible Value"

def main():
  # Create the MIDIFile Object
  MyMIDI = MIDIFile(1)
  
  lookup = {'c5':0, 'c#5':1, 'd5':2, 'd#5':3, 'e5':4, 'f5':5, 'f#5':6, 
  'g5':7, 'g#5':8, 'a5':9, 'a#5':10, 'b5':11, 'c6':12, 'c#6':13, 'd6':14,
  'd#6':15, 'e6':16, 'f6':17, 'f#6':18, 'g6':19, 'g#6':20, 'a6':21, 'a#6':22,
  'b6':23}
 
  matrix = {(x, y):0 for x in range(len(lookup)) for y in range(len(lookup))}

  print len(lookup)

  addNotes(matrix, lookup)

  currNote = 'b5'

  track = 0
  channel = 0
  duration = 1
  volume = 100

  for i in range (0, 100):
    print currNote
    
    time = i
    MyMIDI.addTrackName(track, time, "Sample Track")
    MyMIDI.addTempo(track, time, 140)

    pitch = lookup[currNote] + 72 

    MyMIDI.addNote(track, channel, pitch, time, duration, volume)

    currNote = nextNote(currNote, lookup, matrix) 

  binfile = open("output2.mid", 'wb')
  MyMIDI.writeFile(binfile)
  binfile.close()

if __name__ == '__main__':
  main()
