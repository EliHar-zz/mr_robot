import { MrRobotHtmlPage } from './app.po';

describe('mr-robot-html App', function() {
  let page: MrRobotHtmlPage;

  beforeEach(() => {
    page = new MrRobotHtmlPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});
