FROM ubuntu:18.04

# Change example to the name of your challenge.
ENV USER shorty

WORKDIR /home/$USER

RUN useradd $USER
RUN apt-get update
RUN apt-get install -y xinetd
RUN apt-get update
RUN apt-get install -y build-essential

COPY $USER.xinetd /etc/xinetd.d/$USER
COPY $USER.c /home/$USER/
COPY bosswave.cpp /home/$USER/
COPY encode.c /home/$USER/
COPY encode.h /home/$USER/
COPY $USER.Makefile /home/$USER/Makefile
COPY flag /home/$USER/flag

RUN make -C /home/$USER/

RUN chown -R root:$USER /home/$USER
RUN chmod -R 550 /home/$USER
RUN chmod -x /home/$USER/flag

# Change 1337 to your challenge port
EXPOSE 11032

CMD service xinetd start && sleep 2 && tail -f /var/log/xinetdlog
