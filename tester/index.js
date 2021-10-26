import got from "got"
import FormData from "form-data"
const letters = [ ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'Ä', 'Ö', 'Ü', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',':', '.', '-', '?', '!'];
//  const letters = [ ':', '.', '-', '?', '!'];
//const letters = [ ':'];

function shuffle(array) {
  let currentIndex = array.length,  randomIndex;

  // While there remain elements to shuffle...
  while (currentIndex != 0) {

    // Pick a remaining element...
    randomIndex = Math.floor(Math.random() * currentIndex);
    currentIndex--;

    // And swap it with the current element.
    [array[currentIndex], array[randomIndex]] = [
      array[randomIndex], array[currentIndex]];
  }

  return array;
}

function delay(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
let first = true;
async function testAllLetters () {

  shuffle(letters);

  for (let a of letters) {
    console.log(a)
    const form = new FormData();
    let text = '';
    for (let i=0; i<10; i++) {
      text += a;
    }
    form.append('flaptext', text);
    form.append('devicemode', 'text');
    form.append('flapspeed', '100');
    form.append('alignment', 'center');

    try {
      const res = await got.post('http://', {
        body: form.getBuffer(),
        headers: form.getHeaders(),
      }).text();
      console.log(res);
    } catch (e) {
      console.error('Error: ', e);
    }
    await delay(10000);
    // if(first) {
    //   first = false;
    //   await delay(10000);
    // }
  }
}

testAllLetters();