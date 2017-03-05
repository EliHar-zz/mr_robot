var express = require('express');
var router = express.Router();
var fs = require('fs');

router.get('/', function(req, res, next) {
  res.send('Find all songs here');
});

router.get('/all', function(req, res, next) {
	fs.readFile('/music.json', function (err, data) {
	  if (err) {
		throw err; 
	  }
	  console.log(data.toString());
	});
});

module.exports = router;
