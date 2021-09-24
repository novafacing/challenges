FROM ubuntu:18.04

RUN apt-get update -y && \
    apt-get install patchelf

ENV USER dynamic_type_librarii
WORKDIR /home/$USER
RUN useradd $USER

COPY $USER /home/$USER/$USER
COPY libc.so.6 /home/$USER/
COPY ld-linux-x86-64.so.2 /home/$USER
RUN bash -c 'echo "sun{this_is_a_dummy_flag}" > /home/$USER/flag.txt'
RUN patchelf --set-interpreter /home/$USER/ld-linux-x86-64.so.2 /home/$USER/$USER && patchelf --replace-needed libc.so.6 /home/$USER/libc.so.6 /home/$USER/$USER
WORKDIR /home/$USER

RUN chown -R root:$USER /home/$USER
RUN chmod -R 550 /home/$USER

CMD sleep 5
