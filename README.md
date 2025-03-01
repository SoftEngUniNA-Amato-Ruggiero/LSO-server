# Server
LSO Server - Dialogare coi robot

## Description
Lato server del progetto di LSO anno 2024/2025.
Il client ha il compito di fare da intermediario tra un robot ed una persona, permettendo al robot di fare domande per stabilire quale personalità ha l'interlocutore (livello di estroversione, amicalità, coscienziosità, stabilità emotiva, apertura a nuove esperienze).
Il server riceve le risposte dell'utente e calcola una stima della personalitá da inviare al client.

Il questionario di personalitá utilizzato è il [TIPI](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/).

## Usage

  ### Eseguire localmente:
  `make -j`

  `./main`

  ### Eseguire su container:
  `docker build -t lso-server .`

  `docker run -it --rm -p 9999:9999 --name running-lso-server lso-server`
  
