var fs = require("fs");

if (process.argv.length < 3) {
	console.warn("Bad Command.");
	return;
}
var path = process.argv.splice(2)[0];
try {
	fs.accessSync(path, fs.R_OK);
} catch (err) {
	path = fs.realpathSync("./") + "/" + path;
	try {
		fs.accessSync(path, fs.R_OK);
	} catch (error) {
		console.warn("Bad File.");
		return;
	}
}


console.warn("Input file: " + path);

function fsread(fd, length, cb) {
	var buf = new Buffer(length);
	fs.read(fd, buf, 0, length, null, function (err, num, buf) {
		if (err) {
			console.warn(JSON.stringify(err));
			fs.closeSync(fd);
			return;
		}
		if (num < 1) {
			console.warn("Data lack.");
			fs.closeSync(fd);
			return;
		}
		cb(buf);
	});
}

function readBlock (fd) {
	fsread(fd, 1, function (buf) {
		var tag = buf[0];
		if (tag === 0x2c) {
			console.warn("Image Descriptor");
			fsread(fd, 9, function (buf) {
				console.warn(buf.length + "= 9");
				console.warn("x offset: " + (buf[0] + buf[1]*256));
				console.warn("y offset: " + (buf[2] + buf[3]*256));
				console.warn("Width: " + (buf[4] + buf[5]*256));
				console.warn("Height: " + (buf[6] + buf[7]*256));
				console.warn("Flags: " + buf[8]);
				var flag = buf[8];
				var m = flag /128;
				var pixel = flag%16;
				console.warn("Local Pallet Flag:" + m + ", Bits: " + pixel);
				if (m > 0) {
					var tabs = Math.pow(2, pixel+1);
					
					fsread(fd, tabs*3+2, function (buf) {
						for (var i = 0; i < tabs; i++) {
							console.warn("Color[" + i + "]: " + buf[i*3] + ", " + buf[i*3+1] + ", " + buf[i*3+2]);
						}
						
						console.warn("LZW Codec Length: " + buf[tabs*3]);
						console.warn("First LZW Data Length: " + buf[tabs*3+1]);
						readLZWData(fd, buf[tabs*3+1], function () {
							readBlock(fd);
						});
					});
				} else {
					fsread(fd, 2, function (buff) {
						console.warn("LZW Codec Length: " + buff[0]);
						console.warn("First LZW Data Length: " + buff[1]);
						readLZWData(fd, buff[1], function () {
							readBlock(fd);
						});
					});
				}
			});
		} else if (tag === 0x21) {
			console.warn("Extension Descriptor");
			fsread(fd, 1, function (buf) {
				if (buf[0] === 0xf9) {//Graphic Control Label
					console.warn("Graphic Control Label");
					fsread(fd, 6, function (buf) {
						console.warn("Delay: " + (buf[3]*256+buf[2]));
						if (buf[5] !== 0) {
							console.warn("Bad End.");
						}
						readBlock(fd);
					});
				} else if (buf[0] === 0xFE) {
					console.warn("Quate Block");
					readQuateData(fd, null, function () {});
				} else if (buf[0] === 0x01) {
					console.warn("Plan Text Label Block");
					fsread(fd, 14, function (buf) {
						readQuateData(fd, buf[13], function () {
							readBlock(fd);
						});
					});
				} else if (buf[0] === 0xff) {
					console.warn("Application Extension Block");
					fsread(fd, 13, function (buf) {
						console.warn(buf.toString('ascii', 1, buf.length - 1));
						readLZWData(fd, buf[12], function () {readBlock(fd);});
					});
				} else {
					console.warn("Undefined Descr: " + buf[0]);
					fs.closeSync(fd);
				}
			});
		} else if (tag === 0x3b) {
			console.warn("File End");
			fs.closeSync(fd);
		} else {
			console.warn("Undefined flag: " + tag);
			fs.closeSync(fd);
		}
	});
}

function readLZWData (fd, len, cb) {
	if (len === 0) {
		console.warn("LZW data end.");
		cb();
		return;
	}
	console.warn("This LZW data length:" + len);
	fsread(fd, len+1, function (buf) {
		//TODO: decode
		readLZWData(fd, buf[len], cb);
	});
}
function readQuateData (fd, len, cb) {
	if (len === 0) {
		console.warn("LZW data end.");
		cb();
		return;
	}
	if (len === null) {
		
		fsread(fd, 1, function (buf) {
			readQuateData(fd, buf[0], cb);
		});
	} else 
	fsread(fd, len+1, function (buf) {
		//TODO: decode
		console.warn("Quate: " + buf.toString('ascii'));
		readQuateData(fd, buf[len], cb);
	});
}

fs.open(path, "rs", function (err, fd) {
	if (err) {
		console.warn(JSON.stringify(err));
		return;
	}
	var buf = new Buffer(6);
	fs.read(fd, buf, 0, 6, null, function (err, num, buf) {
		if (err) {
			console.warn(JSON.stringify(err));
			fs.closeSync(fd);
			return;
		}
		if (num < 6) {
			console.warn("Data lack.");
			fs.closeSync(fd);
			return;
		}
		console.warn("version: " + buf.toString('ascii'));
		buf = new Buffer(7);
		fs.read(fd, buf, 0, 7, null, function (err, num, buf) {
			
			if (err) {
				console.warn(JSON.stringify(err));
				fs.closeSync(fd);
				return;
			}
			if (num < 7) {
				console.warn("Data lack.");
				fs.closeSync(fd);
				return;
			}
			console.warn("Width: " + (buf[1] * 256 + buf[0]));
			console.warn("Height: " + (buf[3] * 256 + buf[2]));
			var flag = buf[4];
			console.warn("flag:" + flag);
			var m = Math.floor(flag/128);
			var cr = Math.floor(flag%128/16);
			var s = Math.floor(flag%16/8);
			var pixel = flag%8 + 1;
			var tabs = Math.pow(2, pixel);
			if (m > 0 && tabs > 0) {
				console.warn("Table size: " + tabs);
				buf = new Buffer(tabs * 3);
				fs.read(fd, buf, 0, tabs*3, null, function (err, num, buf) {
			
					if (err) {
						console.warn(JSON.stringify(err));
						fs.closeSync(fd);
						return;
					}
					if (num < tabs*3) {
						console.warn("Data lack.");
						fs.closeSync(fd);
						return;
					}
					for (var i=0; i < tabs; i++) {
						console.warn("Color[" + i + "]: " + buf[i*3] + ", " + buf[i*3+1] + ", " + buf[i*3+2]);
					}
					readBlock(fd);
				});
			} else
			fs.closeSync(fd);
		});
	});
});