# Game

[![Build Status](https://travis-ci.org/2ToThe10th/Game.svg?branch=dev)](https://travis-ci.org/2ToThe10th/Game)


A small multiplayer game with a focus on networking.

The client connects simultaneously via UDP and TCP sockets. Player moves from the client are sent to the server using UDP, and information about all players' movements, processed by the server after physics calculations, is sent back to the client using the UDP. Once a second, the client and server communicate over a TCP connection to compare the hash tree, and the server sends data about parts of the map where the hash did not match. Of course, the server operates entirely in asynchronous mode using epoll.

The game itself is still very simple — the client plays as a circle that can move around a huge map. The map is received from the server upon connection.

--------------------------------------------------------------------------

Небольшая многопользовательская игра с акцентом на сеть

Клиент подключается по UDP и TCP сокету одновременно. По UDP передаются ходы игрока с клиента на сервер и информация о передвижении всех игроков с сервера на клиент, после обсчета физики на сервере. Раз в секунду клиент и сервер общаются по TCP соединению и сравнивают дерево хешей и сервер отправляет данные о частях карты, где хеш не совпал. Конечно же сервер работает полностью в асинхронном режиме с помощью epoll

Сама игра пока очень простая — клиент играет за кружочек, который может перемещаться по огромной карте. Карта приходит с сервера при подключении
