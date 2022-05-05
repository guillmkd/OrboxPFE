/**
 * Created by age2pierre on 05/02/17.
 */
let router = require('express').Router();

router.get('/', (req, res, next) => {
  res.render('home');
});

module.exports = router;