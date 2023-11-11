# Tech Girls - Tech Traffic Solutions 
Com Smart Cities, em uma pesquisa, entendemos que o maior problema que atinge São Paulo e todos que aqui habitam é justamente o trânsito. Então, começamos a pensar em formas de melhorias do trânsito unindo a tecnologia a nosso favor.

## O que faremos para melhorar o trânsito?

Com Python, criamos uma inteligência artificial chamada Tiana que foca na melhor experiência do usuário e que se comunica diretamente com ele. Cada usuário tem a sua preferência, e além das configurações que o próprio usuário poderá colocar manualmente, a ideia é fazer um machine learning, para que cada usuário tenha uma experiência única. <br>

Para nos auxiliar nesse trabalho, iremos utilizar o ESP32, que conecta a bluetooth e poderíamos, futuramente, fazer uma conexão com a Tiana que avisaria em tempo real se um local está congestionado ou não. Além da própria comunicação com os usuários com a aplicação. E, antes de avançarmos para o tópico que fala melhor sobre os sensores que utilizaremos e o que pretendemos fazer futuramente com o ESP32, acesse ao <a href="https://github.com/FIAP-Tech-Girls/Challenge-Python">repositório da Tiana</a> aqui, para saber mais informações!

## Gostaria de mais detalhes sobre o projeto, como posso saber?

Você pode acessar nosso vídeo Pitch através do link: https://youtu.be/Yfn0zL2YrBM que conta de forma profissional e empreendedora sobre o problema, a nossa solução e sobre quem somos e um protótipo no Tinkercad.

Em relação a um protótipo novo, gravamos de uma forma simples mostrando como é o ESP32 funcionando na prática, além de termos mostrado no NEXT com as ruas funcionando. Link do vídeo para o Youtube: https://youtube.com/shorts/SEGctKBVCnE

## Como o ESP32 nos auxiliou nesse processo?

Nos utilizamos duas bibliotecas, a Adafruit VL53L0X para o sensor de distância e na versão IOT, que você pode encontrar o CodigoFinalIOT.ino, utilizamos a biblioteca Wifi e PubSubClient.

Para construir nosso protótipo nos utilizamos:

<ul>
  <li>Sensor de distância: responsável por realizar a contagem de carros, dada distância calculada nos testes (acima de 200 não há carros);</li>
  <li>Semafóro de carros: composto por um led vermelho, um amarelo e um verde;</li>
  <li>Semafóro de pedestres: composto por um led vermelho e um verde;</li>
  <li>Botão: que os pedestres podem apertar para abrir o semafóro de pedestres e fechar o de carros caso esteja demorando muito para abrir para eles.</li>
</ul>

Nesse projeto, utilizamos o ESP32 para nos auxiliar nesse protótitpo com o IoT.

## Como baixar um servidor IoT Fiware?

Fizemos essa instalação na sprint três e explicamos detalhe por detalhe no repositório que você pode encontrar <a href="https://github.com/FIAP-Tech-Girls/Challenge-Edge">aqui<\a>

## Em relação aos códigos, onde devemos ir?

Com o IoT, o código é: codigoESP32IOT.ino
O código final que foi apresentado no Next sem o IoT, está na pasta de CodigoFinal que possui um código de .ino dentro
Os demais códigos foram testes que realizamos conforme fomos construindo. Não é necessário ver, apenas se houver curiosidade!

## Copyright

ATENÇÃO: O programa deve ser utilizado EXCLUSIVAMENTE como forma de estudos para aqueles que baixarem sem serem as desenvolvedoras.

Copyright ©️ Anny, Camila, Leticia e Luana.
