#import the midi util library and the random library
from midiutil.MidiFile import MIDIFile
import random
import sys

''' 
This function uses the current note and the first order Markov matrix
to determine the next note to play
'''
def nextNote(currentNote, matrix):
  return random.choice(matrix[currentNote])

'''
This function creates a dictionary that maps each note string to its
corresponding midi value
'''
def createLookup():
  noteLookup = {'c0':0, 'c#0':1, 'd0':2, 'd#0':3, 'e0':4, 'f0':5, 'f#0':6, 
  'g0':7, 'g#0':8, 'a0':9, 'a#0':10, 'b0':11, 'c1':12, 'c#1':13, 'd1':14, 
  'd#1':15, 'e1':16, 'f1':17, 'f#1':18, 'g1':19, 'g#1':20, 'a1':21, 'a#1':22, 
  'b1':23, 'c2':24, 'c#2':25, 'd2':26, 'd#2':27, 'e2':28, 'f2':29, 'f#2':30, 
  'g2':31, 'g#2':32, 'a2':33, 'a#2':34, 'b2':35, 'c3':36, 'c#3':37, 'd3':38, 
  'd#3':39, 'e3':40, 'f3':41, 'f#3':42, 'g3':43, 'g#3':44, 'a3':45, 'a#3':46, 
  'b3':47, 'c4':48, 'c#4':49, 'd4':50, 'd#4':51, 'e4':52, 'f4':53, 'f#4':54, 
  'g4':55, 'g#4':56, 'a4':57, 'a#4':58, 'b4':59, 'c5':60, 'c#5':61, 'd5':62, 
  'd#5':63, 'e5':64, 'f5':65, 'f#5':66, 'g5':67, 'g#5':68, 'a5':69, 'a#5':70, 
  'b5':71, 'c6':72, 'c#6':73, 'd6':74, 'd#6':75, 'e6':76, 'f6':77, 'f#6':78, 
  'g6':79, 'g#6':80, 'a6':81, 'a#6':82, 'b6':83, 'c7':84, 'c#7':85, 'd7':86, 
  'd#7':87, 'e7':88, 'f7':89, 'f#7':90, 'g7':91, 'g#7':92, 'a7':93, 'a#7':94, 
  'b7':95, 'c8':96, 'c#8':97, 'd8':98, 'd#8':99, 'e8':100, 'f8':101, 'f#8':102,
  'g8':103, 'g#8':104, 'a8':105, 'a#8':106, 'b8':107, 'c9':108, 'c#9':109, 
  'd9':110, 'd#9':111, 'e9':112, 'f9':113, 'f#9':114, 'g9':115, 'g#9':116, 
  'a9':117, 'a#9':118, 'b9':119, 'c10':120, 'c#10':121, 'd10':122, 'd#10':123, 
  'e10':124, 'f10':125, 'f#10':126, 'g10':127}

  return noteLookup

'''
This function adds the notes from the appropriate text file to the first
order Markov matrix
'''
def addToMatrix(noteList, matrix):
  i = 0
  while i < len(noteList) - 1:
    matrix.setdefault(noteList[i],[]).append(noteList[i + 1])
    i += 1

'''
Main function that reads note/chord strings from a text file and writes
output to a midi file
'''
def main():
  # Check if correct number of command line args are present
  if len(sys.argv) == 2:
    songFile = sys.argv[1]
  else:
    sys.exit("Missing an input text file with notes")

  # Open text file containing song notes
  noteFile = open(songFile, 'r')

  # Read in notes from text file
  noteLines = noteFile.readlines()

  noteFile.close();

  # Separate notes from each line of the text file and add to notes 
  notes = []
  for line in noteLines:
    notes = notes + line.split()

  # Initialize matrix and add notes
  markovMatrix = {}
  addToMatrix(notes, markovMatrix)

  # Randomly select first note to play
  currNote = random.choice(notes)

  # Create the MIDIFile Object
  midFile = MIDIFile(1)

  lookup = createLookup()

  # Midi file info
  track = 0
  channel = 0
  duration = 1
  volume = 100

  # Add note to Midi file for the first note
  time = 0
  midFile.addTrackName(track, time, "Main Track")
  midFile.addTempo(track, time, 140)
  
  # Loop and generate the next notes determined by the currently playing note
  for i in xrange(200):
    print currNote + ', '
  
    time = i 
 
    # comma means it is a chord.  Split into individual notes from chord
    if ',' in currNote:
      chord = currNote.split(',')

      # Loop through notes in chord and determine duration note is played
      for note in chord:
        notePlusLen = note.split('-')
        if len(notePlusLen) == 2:
          duration = float(notePlusLen[1])
        
        # Lookup midi value of note
        pitch = lookup[notePlusLen[0]]

        # Add note to midi file
        midFile.addNote(track, channel, pitch, time, duration, volume)
    else:
      # '-' separates note and duration
      notePlusLen = currNote.split('-')
      if len(notePlusLen) == 2:
        duration = float(notePlusLen[1])

      # Lookup midi value of note
      pitch = lookup[notePlusLen[0]]

      # Add note to midi file
      midFile.addNote(track, channel, pitch, time, duration, volume)

    # Determine the next note to play using the current note and matrix
    currNote = nextNote(currNote, markovMatrix)

  # Write to midi file and close
  binfile = open("bond.mid", 'wb')
  midFile.writeFile(binfile)
  binfile.close()

if __name__ == '__main__':
  main()
