all: monster

monster:
	docker build . -t $@
	docker run --name $@ -itd -p 11031:11031 $@
	docker cp $@:/home/$@/$@ ./$@.dist
