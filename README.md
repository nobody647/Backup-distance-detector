# Backup-distance-detector
A little project for my car using an ESP8266


This project has two parts (and two repos)
The other part is over at https://github.com/nobody647/Distance-Beeper


This repo has stuff for an esp8266 and a distance sensor. It will sleep until it sees a wifi network, in which case it acts as a server on 192.158.1.50. It listens for any TCP packet (doesnt matter what the content is) and replies with a reading from the distance sensor)
