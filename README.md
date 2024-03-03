# 🌐Tuple space middleware for distributed computing🔀

To repozytorium przechowuje materiały zgromadzone przy pracy na projektem "Tuple space middleware for distributed computing" w ramach przedmiotu PSIR.

## 🖥️ Prezentacja rozwiązania: 

![Działający system z dwoma aplikacjami](full-app12.gif)

Na powyższym zrzucie przedstawiono działanie systemu z dwoma uruchomionymi aplikacjami jednocześnie:
- APP 1: Rozproszone obliczanie liczb pierwszych
- APP 2: IoT zdecentralizowany system kontroli wilgotności powietrza

(W lewej części widoczny jest serwer przestrzeni krotek, a w prawej połączenia SSH do emulatorów Arduino, na których uruchomione są aplikacje.)


## 📋 Rzeczy do zrobienia
lista będzie na bierząco uzupełniana

- [x] kod sprawdzający czy liczba jest pierwsza
- [ ] podstawowa struktura tuple space
- [x] podstawowa struktura managera
- [ ] test komunikacji tuple space <-> manager
- [x] postawowa struktura worker
- [ ] test komunikacji tuple space <-> 3x worker
- [ ] opracowanie binarnego protokołu warstwy aplikacyjnej APL
- [ ] implementacja APL
- [ ] implementacja serializacji (zamiana wartości przy przesyłaniu i obieraniu)

## 🧭 Nawigacja

- tuple-space - oprogramowanie odpowiedzialne za obsulgę przestrzeni krotek
- worker - program wykonujący przydzielone zadania umieszczone w przestrzeni krotek, uruchamiany na emulatorze Arduino
- manager - program tworzący zadania dla "workerów", umieszcza je w przestzreni krotek
- common - miejsce na przechowywanie współdzielonego kodu między różnymi modułami

## 🖐️ Autorzy

- Jakub Marchut [@kubamarchut](https://www.github.com/kubamarchut)
- Katarzyna Stec [@KasiaStec](https://www.github.com/KasiaStec)
- Jakub Kadłubowski [@kubakadlubowski](https://www.github.com/kadlub)

## 📃 Dokumentacja

[Dokumentacja - overleaf](https://www.overleaf.com/read/jdprzxtqqcqg#d17f5a)
