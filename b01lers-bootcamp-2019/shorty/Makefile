all: shorty

shorty:
	docker build . -t $@
	docker run --name $@ --restart always -itd -p 11032:11032 $@
	docker cp $@:/home/$@/$@ ./$@.dist

.PHONY: shorty
