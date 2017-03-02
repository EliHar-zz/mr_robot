export class ContactItem {
  constructor(private name : string, private numbers : string[]){}

  public get getName() : string {
    return this.name;
  }

  public get getNumbers() : string[] {
    return this.numbers;
  }
}
