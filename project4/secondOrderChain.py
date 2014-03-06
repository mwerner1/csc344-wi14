#######################################################################
# Program that generates a pseudo-random song using a 
# second-order Markov Chain and the James Bond theme song
# notes to determine the probablity of a note to be played using
# the previous two notes from the theme song
#######################################################################

#import the midi util library and the random library
from midiutil.MidiFile import MIDIFile
import random
import sys

# Adds the occurrences of each note after the previous two notes
# from the James Bond theme song.  These are used to determine
# the probability of each note to be played
def addNotes(matrix, lookup):
  # Intro to James Bond theme song
  matrix[(lookup['b5'], lookup['e5']), lookup['c6']] += 1
  matrix[(lookup['e5'], lookup['c6']), lookup['e5']] += 1
  matrix[(lookup['c6'], lookup['e5']), lookup['c#6']] += 1
  matrix[(lookup['e5'], lookup['c#6']), lookup['b5']] += 1
  matrix[(lookup['c#6'], lookup['b5']), lookup['e5']] += 1
  matrix[(lookup['b5'], lookup['e5']), lookup['c6']] += 1
  matrix[(lookup['e5'], lookup['c6']), lookup['e5']] += 1
  matrix[(lookup['c6'], lookup['e5']), lookup['c#6']] += 1
  matrix[(lookup['e5'], lookup['c#6']), lookup['e5']] += 1
  matrix[(lookup['c#6'], lookup['e5']), lookup['f#5']] += 1
  matrix[(lookup['e5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['e5']] += 1
  matrix[(lookup['f#5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['g5']] += 1
  matrix[(lookup['e5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['f#5']] += 1
  matrix[(lookup['g5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['e5']] += 1
  matrix[(lookup['f#5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['g5']] += 1
  matrix[(lookup['e5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['f#5']] += 1
  matrix[(lookup['g5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['f#5']] += 1
  matrix[(lookup['f#5'], lookup['f#5']), lookup['e5']] += 1
  matrix[(lookup['f#5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['e5']] += 1
  matrix[(lookup['e5'], lookup['e5']), lookup['g5']] += 1
  matrix[(lookup['e5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['g5']] += 1
  matrix[(lookup['g5'], lookup['g5']), lookup['e5']] += 1

  # Main melody
  matrix[(lookup['g5'], lookup['e5']), lookup['g5']] += 1
  matrix[(lookup['e5'], lookup['g5']), lookup['d#6']] += 1
  matrix[(lookup['g5'], lookup['d#6']), lookup['d6']] += 1
  matrix[(lookup['d#6'], lookup['d6']), lookup['g5']] += 1
  matrix[(lookup['d6'], lookup['g5']), lookup['a#5']] += 1
  matrix[(lookup['g5'], lookup['a#5']), lookup['b5']] += 1
  matrix[(lookup['a#5'], lookup['b5']), lookup['g5']] += 1
  matrix[(lookup['b5'], lookup['g5']), lookup['a5']] += 1
  matrix[(lookup['g5'], lookup['a5']), lookup['g5']] += 1
  matrix[(lookup['a5'], lookup['g5']), lookup['f#5']] += 1
  matrix[(lookup['g5'], lookup['f#5']), lookup['b5']] += 1
  matrix[(lookup['f#5'], lookup['b5']), lookup['e6']] += 1
  matrix[(lookup['b5'], lookup['e6']), lookup['c#6']] += 1

  # Main melody a second time around with different ending
  matrix[(lookup['e6'], lookup['c#6']), lookup['e5']] += 1
  matrix[(lookup['c#6'], lookup['e5']), lookup['g5']] += 1
  matrix[(lookup['e5'], lookup['g5']), lookup['d#6']] += 1
  matrix[(lookup['g5'], lookup['d#6']), lookup['d6']] += 1
  matrix[(lookup['d#6'], lookup['d6']), lookup['g5']] += 1
  matrix[(lookup['d6'], lookup['g5']), lookup['a#5']] += 1
  matrix[(lookup['g5'], lookup['a#5']), lookup['b5']] += 1
  matrix[(lookup['a#5'], lookup['b5']), lookup['g5']] += 1
  matrix[(lookup['b5'], lookup['g5']), lookup['a5']] += 1
  matrix[(lookup['g5'], lookup['a5']), lookup['g5']] += 1
  matrix[(lookup['a5'], lookup['g5']), lookup['f#5']] += 1
  matrix[(lookup['g5'], lookup['f#5']), lookup['b5']] += 1
  matrix[(lookup['f#5'], lookup['b5']), lookup['d#6']] += 1
  matrix[(lookup['b5'], lookup['d#6']), lookup['e6']] += 1

# Determines the next note to be played
def nextNote(prevNote, currNote, lookup, matrix):
  rowSum = 0
  intermediateSum = 0

  # Sum of the occurrences of all notes played after the the previous note
  # and current note
  for i in range(len(lookup)):
    rowSum += matrix[(lookup[prevNote], lookup[currNote]), i]

  # Randomly select a number from 1-rowSum
  randCount = random.randrange(1, rowSum + 1)

  # Determine note to be played using the randCount
  for i in range(len(lookup)):
    intermediateSum += matrix[(lookup[prevNote], lookup[currNote]), i]
    if (intermediateSum >= randCount):
      for note, val in lookup.iteritems():
        if val == i:
          return note
  raise RuntimeError, "Impossible Value"

# Main function
def main():
  # Create the MIDIFile Object
  MyMIDI = MIDIFile(1)
  
  # Create lookup dictionary that maps note string literals to indexes
  lookup = {'c5':0, 'c#5':1, 'd5':2, 'd#5':3, 'e5':4, 'f5':5, 'f#5':6, 
  'g5':7, 'g#5':8, 'a5':9, 'a#5':10, 'b5':11, 'c6':12, 'c#6':13, 'd6':14,
  'd#6':15, 'e6':16, 'f6':17, 'f#6':18, 'g6':19, 'g#6':20, 'a6':21, 'a#6':22,
  'b6':23}

  # Create matrix for second-order Markov Chain and initialize all cells to 0
  matrix = {((x, y), z):0 for x in range(len(lookup)) for y in 
  range(len(lookup)) for z in range (len(lookup))}

  # Add notes from James Bond theme song
  addNotes(matrix, lookup)

  # Hard code first two notes to be played
  prevNote = 'b5'
  currNote = 'e5'

  # Midi file info
  track = 0
  channel = 0
  duration = 1
  volume = 100

  # Add note to Midi file for the first note (the "prevNote")
  time = 0
  MyMIDI.addTrackName(track, time, "Main Track")
  MyMIDI.addTempo(track, time, 140)
  pitch = lookup[prevNote] + 72
  MyMIDI.addNote(track, channel, pitch, time, duration, volume)
  
  # Generate 200 "ticks" of Midi notes
  for i in range (1, 200):
    print 'currNote: ' + currNote
    
    # Add the current note to the Midi file
    time = i
    MyMIDI.addTrackName(track, time, "Main Track")
    MyMIDI.addTempo(track, time, 140)
    pitch = lookup[currNote] + 72 
    MyMIDI.addNote(track, channel, pitch, time, duration, volume)

    # recalculate previous and current notes
    tempNote = currNote
    currNote = nextNote(prevNote, currNote, lookup, matrix) 
    prevNote = tempNote


  binfile = open("bondOutput.mid", 'wb')
  MyMIDI.writeFile(binfile)
  binfile.close()

if __name__ == '__main__':
  main()
