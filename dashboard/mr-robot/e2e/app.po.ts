import { browser, element, by } from 'protractor';

export class MrRobotPage {
  navigateTo() {
    return browser.get('/');
  }

  getParagraphText() {
    return element(by.css('mr-root h1')).getText();
  }
}
