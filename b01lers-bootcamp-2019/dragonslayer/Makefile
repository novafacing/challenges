all: dragonslayer

dragonslayer:
	docker build . -t $@
	docker run --name $@ --restart always -itd -p 11030:11030 $@ 
	docker cp $@:/home/$@/$@ ./$@.dist

.PHONY: dragonslayer
