## Simple web-based signal viewer

This example encapsulates a simple JavaScript-enabled web-based viewer for messages exchanged in an OD4Session.

```
docker run --rm -ti --net=host -p 8080:8080 chrberger/dit168-signal-viewer:v0.0.1 --cid=111
```

The Docker image is provided for amd64 platforms. To change to a different CID session, simply provide a different parameter at the end.

To change the messages that this application can understand, simply modify the messages.odvd (or let it dynamically load from somewhere :-)
